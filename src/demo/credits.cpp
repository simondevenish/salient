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
#include "credits.hpp"

constexpr auto CREDITS_TEXT =
    "Salient demo\n"
    "Copyright (c) 2009-2022, Jice and the Salient contributors\n"
    "\n"
    "Powered by:\n"
    "libtcod " TCOD_STRVERSION
    "\n"
    "Copyright (c) 2008-2022, Jice and the Salient contributors\n"
    "\n"
    "Simple DirectMedia Layer 1.2.12\n"
    "Copyright (c) Sam Lantinga";

void Credits::render() {
  constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};
  constexpr auto BLACK = tcod::ColorRGB{0, 0, 0};
  constexpr std::array DECORATOR = {0x250c, 0x2500, 0x2510, 0x2502, 0x20, 0x2502, 0x2514, 0x2500, 0x2518};
  tcod::draw_frame(credits, {0, 0, 48, 11}, DECORATOR, WHITE, BLACK);
  tcod::print_rect(credits, {1, 1, 46, 9}, CREDITS_TEXT, WHITE, {}, TCOD_CENTER);
  tcod::blit(*TCODConsole::root, credits, {rect.x, rect.y}, {}, 1.0f, 0.75f);
}
