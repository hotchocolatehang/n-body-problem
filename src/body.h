/**
MIT License

Copyright (c) 2019 Alexandr Krikun

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
 */

#ifndef BODY_H
#define BODY_H

#include <array>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "abstract_body.h"

namespace n_body_problem {

struct Body : public AbstractBody, public sf::Drawable {
public:
  Body();
  Body(double x_pos, double y_pos, double x_vel, double y_vel, long _mass, double radius, unsigned _r, unsigned _g, unsigned _b);
	Body(vector2<double> _pos_curr, vector2<double> _velocity, long _mass, double radius, sf::Color _color);
  Body(Body const& other);
  ~Body() override;
  void MoveToNextTimePoint() override;

  vector2<double> pos_old;
  static size_t traj_length;
  sf::Color color;
  sf::CircleShape shape;

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  // TODO: change vector to dynarray
  std::vector<sf::Vertex> trajectory_;
  size_t traj_pos_;
  size_t traj_begin_;
}; // struct Body

} // namespace n_body_problem

#endif