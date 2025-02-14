/* BSD 3-Clause License
 *
 * Copyright © 2008-2022, Jice, Odiminox and the salient contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "config/config.hpp"

#include <stdio.h>

#include <array>
#include <libtcod/libtcod.hpp>

#include "logger/log.hpp"

namespace config {
static constexpr std::array logLevelName = {"info", "notice", "warning", "error", "fatal error", "none"};

void Config::load(std::filesystem::path path) {
  static bool loaded = false;
  TCODParser parser;
  logger::Log::openBlock("Config::load | Loading configuration variables.");
  if (loaded && Config::fileName == path) {
    logger::Log::notice("Config::load | Configuraion variables have been loaded previously. Aborting.");
    logger::Log::closeBlock(logger::LOGRESULT_FAILURE);
    return;
  }

  Config::fileName = path;

  // register configuration variables
  parser.newStructure("config")
      ->addProperty("rootWidth", TCOD_TYPE_INT, true)
      ->addProperty("rootHeight", TCOD_TYPE_INT, true)
      ->addProperty("fontID", TCOD_TYPE_INT, true)
      ->addProperty("fullScreen", TCOD_TYPE_BOOL, true)
      ->addProperty("logLevel", TCOD_TYPE_STRING, true)
      // optional custom font directory
      ->addProperty("fontDir", TCOD_TYPE_STRING, false)
      // optional module chaining
      ->addProperty("moduleChain", TCOD_TYPE_STRING, false);

  // check if the config file exists
  if (!std::filesystem::exists(path)) {
    logger::Log::notice(
        "Configuration file %s is bad or missing. Attempting to create a new one.", path.string().c_str());
    // assign defaults
    rootWidth = 80;
    rootHeight = 60;
    fontID = 0;
    fullScreen = false;
    logLevel = LOGLEVEL_ERROR;
    fontDir = "data/img";
    Config::save();
  }

  // run the parser
  parser.run(path.string().c_str(), NULL);

  // assign parsed values to class variables
  rootWidth = parser.getIntProperty("config.rootWidth");
  rootHeight = parser.getIntProperty("config.rootHeight");
  fontID = parser.getIntProperty("config.fontID");
  fullScreen = parser.getBoolProperty("config.fullScreen");
  fontDir = "data/img";  // default value
  if (parser.hasProperty("config.fontDir")) fontDir = parser.getStringProperty("config.fontDir");
  moduleChain = "";
  if (parser.hasProperty("config.moduleChain")) moduleChain = parser.getStringProperty("config.moduleChain");
  // set log level
  {
    std::string configLogLevel = "info";
    if (parser.hasProperty("config.logLevel")) configLogLevel = parser.getStringProperty("config.logLevel");
    for (int i = 0; i <= static_cast<int>(LOGLEVEL_NONE); ++i) {
      if (configLogLevel == logLevelName.at(i)) logLevel = static_cast<LogLevel>(i);
    }
  }
  loaded = true;
  logger::Log::closeBlock(logger::LOGRESULT_SUCCESS);
}

void Config::save() {
  FILE* out;
  std::string modC = "";

  logger::Log::info("Config::save | Saving configuration variables.");

  out = fopen(fileName.string().c_str(), "w");

  if (moduleChain != NULL) {
    modC += "  moduleChain = \"";
    modC += moduleChain;
    modC += "\"\n";
  }

  fprintf(
      out,
      "/*\n"
      " * UMBRA CONFIGURATION FILE\n"
      " *\n"
      " * rootWidth (integer): width of the root console in cells\n"
      " * rootHeight (integer): height of the root console in cells\n"
      " * fontID (integer): the ID of the font that is to be used\n"
      " * fullScreen (boolean): whether the application should run in full screen.\n"
      " *                       * true = run in full screen mode\n"
      " *                       * false = run in windowed mode (default)\n"
      " * logLevel (string): which messages are supposed to be logged.\n"
      " *                    * \"info\" = all messages down to the info level\n"
      " *                                 (full debug mode)\n"
      " *                    * \"notice\" = all debug messages down to notices\n"
      " *                    * \"warning\" = all debug messages down to warnings\n"
      " *                    * \"error\" = log only errors and fatal errors\n"
      " *                                  (standard debug mode, default)\n"
      " *                    * \"fatal error\" = log only fatal errors\n"
      " *                    * \"none\" = don't create a logfile at all\n"
      " *                                 (debug mode off)\n"
      " * fontDir (string): the directory containing font files\n"
      " * moduleChain (string): the module chain to load (optional)\n"
      " */\n"
      "\n"
      "config {\n"
      "  rootWidth = %d\n"
      "  rootHeight = %d\n"
      "  fontID = %d\n"
      "  fullScreen = %s\n"
      "  logLevel = \"%s\"\n"
      "  fontDir = \"%s\"\n"
      "%s"
      "}\n",
      rootWidth,
      rootHeight,
      fontID,
      (TCODConsole::isFullscreen() ? "true" : "false"),
      logLevelName.at(logLevel),
      fontDir.string().c_str(),
      modC.c_str());

  fclose(out);
}

void Config::registerFont(const base::Font& new_font) {
  logger::Log::info("Config::registerFont | Registered a font.");
  fonts.emplace_back(new_font);
}

bool Config::activateFont(int shift) {
  int s = CLAMP(-1, 1, shift);
  // check if there are any registered fonts
  if (fonts.size() == 0)
    return false;  // can happen if a user uses the default terminal.png without registering any font
  // check if the requested ID isn't out of range
  if (fontID + s < 0 || fontID + s >= static_cast<int>(fonts.size())) return false;
  // check if the font needs changing at all
  if (font != NULL && s == 0) return false;
  // register the font
  font = &fonts.at(fontID + s);
  fontID += s;
  return true;
}
}  // namespace config
