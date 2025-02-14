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
#include "events/callback.hpp"

#include <libtcod/libtcod.hpp>

#include "engine/engine.hpp"
#include "module/module.hpp"

namespace events {
// quit program
CallbackQuit::CallbackQuit() { key = {TCODK_F4, 0, true, false, false}; }

void CallbackQuit::action() { getEngine()->deactivateAll(); }

// switch fullscreen
CallbackFullscreen::CallbackFullscreen() { key = {TCODK_ENTER, '\r', true, false, false}; }

void CallbackFullscreen::action() { TCODConsole::setFullscreen(!TCODConsole::isFullscreen()); }

// save screenshot
CallbackScreenshot::CallbackScreenshot() { key = {TCODK_PRINTSCREEN, 0, false, false, false}; }

void CallbackScreenshot::action() { TCODSystem::saveScreenshot(NULL); }

// switch font up
CallbackFontUp::CallbackFontUp() { key = {TCODK_PAGEUP, 0, false, false, false}; }

void CallbackFontUp::action() {
  if (getEngine()->activateFont(1)) getEngine()->reinitialise();
}

// switch font down
CallbackFontDown::CallbackFontDown() { key = {TCODK_PAGEDOWN, 0, false, false, false}; }

void CallbackFontDown::action() {
  if (getEngine()->activateFont(-1)) getEngine()->reinitialise();
}

// pause the program
CallbackPause::CallbackPause() { key = {TCODK_PAUSE, 0, false, false, false}; }

void CallbackPause::action() { getEngine()->setPause(!getEngine()->getPause()); }

// pause the program
CallbackSpeedometer::CallbackSpeedometer() { key = {TCODK_F5, 0, false, false, false}; }

void CallbackSpeedometer::action() {
  if (getEngine()->getModule(engine::INTERNAL_SPEEDOMETER)->getActive()) {
    getEngine()->deactivateModule(engine::INTERNAL_SPEEDOMETER);
  } else {
    getEngine()->activateModule(engine::INTERNAL_SPEEDOMETER);
  }
}
}  // namespace events
