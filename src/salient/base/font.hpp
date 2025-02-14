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
#pragma once
#include <libtcod/console_types.h>

#include <cassert>
#include <string>

namespace base {
class Font {
 public:
  Font() = default;
  /// @brief Initialises a font.
  /// @param columns number of columns of characters in the font image file
  /// @param rows number of rows of characters in the font image file
  /// @param filename the filename of the font image file
  /// @param flags font layout flags
  Font(int columns, int rows, std::string filename, int flags = TCOD_FONT_LAYOUT_TCOD)
      : filename_{std::move(filename)}, columns_{columns}, rows_{rows}, flags_{flags} {}
  /**
   * Initialises a font. Used for manual initialisation (ie, when automatic font detection isn't used).
   */
  [[deprecated("Construct this object instead.")]] void initialise(
      int c, int r, const char* fn, int f = TCOD_FONT_LAYOUT_TCOD) {
    assert(fn);
    *this = Font{c, r, fn, f};
  }
  /**
   * Gets the font image's filename
   * @return the font image's filename
   */
  const char* filename() const noexcept { return filename_.c_str(); }
  /**
   * Gets the font image's number of columns
   * @return the font image's number of columns
   */
  int columns() const noexcept { return columns_; }
  /**
   * Gets the font image's number of rows
   * @return the font image's number of rows
   */
  int rows() const noexcept { return rows_; }
  /**
   * Gets the font image's layout flags
   * @return the font image's layout flags
   */
  int flags() const noexcept { return flags_; }

 private:
  std::string filename_ = "NULL";
  int columns_ = 0;
  int rows_ = 0;
  int flags_ = 0;
};
}  // namespace base
