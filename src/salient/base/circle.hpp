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
#include "base/point.hpp"

namespace base {
class Circle {
 public:
  Circle() = default;
  Circle(int r) : r{r} {}
  Circle(int x, int y) : x{x}, y{y} {}
  Circle(int x, int y, int r) : x{x}, y{y}, r{r} {}
  Circle(const base::Point& p) : x{p.x}, y{p.y} {}
  Circle(const base::Point& p, int r) : x{p.x}, y{p.y}, r{r} {}
  /**
   * Sets the circle's position.
   * @param x the circle centre's <i>x</i> coordinate
   * @param y the circle centre's <i>y</i> coordinate
   */
  inline void setPos(int new_x, int new_y) {
    x = new_x;
    y = new_y;
  }
  /**
   * Sets the circle's position.
   * @param p the point whose coordinates are to be come the circle's centre
   */
  inline void setPos(const base::Point& p) {
    x = p.x;
    y = p.y;
  }
  /**
   * Sets the circle's radius, keeping the position unchanged.
   * @param r the circle's radius
   */
  inline void setRadius(int new_r) { r = new_r; }
  /**
   * Sets the circle's position and radius.
   * @param x the circle centre's <i>x</i> coordinate
   * @param y the circle centre's <i>y</i> coordinate
   * @param r the circle's radius
   */
  inline void set(int new_x, int new_y, int new_r) {
    setPos(new_x, new_y);
    setRadius(new_r);
  }
  /**
   * Sets the circle's position and radius.
   * @param p the point whose coordinates are to be come the circle's centre
   * @param r the circle's radius
   */
  inline void set(const base::Point& p, int new_r) {
    setPos(p);
    setRadius(new_r);
  }
  /**
   * Checks whether a set of coordinates is contained within the circle.
   * @param px the <i>x</i> coordinate to be checked
   * @param py the <i>y</i> coordinate to be checked
   * @return <code>true</code> if the coordinates are within the circle, <code>false</code> otherwise
   */
  inline bool contains(int px, int py) { return (x - px) * (x - px) + (y - py) * (y - py) <= r * r; }
  /**
   * Checks whether a point is contained within the circle.
   * @param p the point to be checked
   * @return <code>true</code> if the point is within the circle, <code>false</code> otherwise
   */
  inline bool contains(const base::Point& p) { return (((x - p.x) * (x - p.x)) + ((y - p.y) * (y - p.y))) <= (r * r); }
  /**
   * Sets the <code>mouseHover</code> and <code>mouseDown</code> statuses for the circle.
   * @param px the <i>x</i> coordinate to be checked
   * @param py the <i>y</i> coordinate to be checked
   * @param ms a reference to the mouse event object
   */
  inline void mouse(int px, int py, TCOD_mouse_t& m) {
    mouseHover = contains(px, py);
    mouseDown = mouseHover & m.lbutton;
  }
  /**
   * Sets the <code>mouseHover</code> and <code>mouseDown</code> statuses for the circle.
   * @param p the point to be checked
   * @param ms a reference to the mouse event object
   */
  inline void mouse(const base::Point& p, TCOD_mouse_t& m) {
    mouseHover = contains(p);
    mouseDown = mouseHover & m.lbutton;
  }
  int x{0};
  int y{0};
  int r{0};
  bool mouseHover{false};
  bool mouseDown{false};
};
}  // namespace base
