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

#include "body.h"

#include <array>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "physics/abstract_body.h"

namespace nbp = n_body_problem;

size_t nbp::Body::traj_length = 100;  // default hard-coded length of trajectory

nbp::Body::Body()
    : AbstractBody(),
      pos_old(0, 0),
      color(0, 0, 0),
      trajectory_(traj_length * 5),
      traj_pos_(0),
      traj_begin_(0){

      };

nbp::Body::Body(double x_pos, double y_pos, double x_vel, double y_vel,
                long _mass, double radius, unsigned _r, unsigned _g,
                unsigned _b)
    : AbstractBody(x_pos, y_pos, x_vel, y_vel, _mass),
      pos_old(x_pos, y_pos),
      color(_r, _g, _b),
      shape(radius, 10),
      trajectory_(traj_length * 5),
      traj_pos_(0),
      traj_begin_(0) {
  shape.setFillColor(color);
  trajectory_[0] = sf::Vector2f(pos_curr.x, pos_curr.y);
};

nbp::Body::Body(vector2<double> _pos_curr, vector2<double> _velocity,
                long _mass, double radius, sf::Color _color)
    : AbstractBody(_pos_curr, _velocity, _mass),
      pos_old(_pos_curr),
      color(_color),
      shape(radius, 10),
      trajectory_(traj_length * 5),
      traj_pos_(0),
      traj_begin_(0) {
  shape.setFillColor(color);
  trajectory_[0] = sf::Vector2f(pos_curr.x, pos_curr.y);
};

nbp::Body::Body(Body const& other)
    : AbstractBody(other),
      pos_old(other.pos_old),
      color(other.color),
      shape(other.shape),
      trajectory_(other.trajectory_),
      traj_pos_(0),
      traj_begin_(other.traj_begin_){

      };

nbp::Body::~Body(){

};

void nbp::Body::MoveToNextTimePoint() {
  pos_old = pos_curr;
  AbstractBody::MoveToNextTimePoint();
  traj_pos_++;
  if (traj_pos_ >= traj_length) traj_begin_++;
  if (traj_pos_ >= traj_length * 5) {
    for (size_t i = 0; i < traj_length; i++)
      trajectory_[i] = trajectory_[i + traj_length * 4];
    traj_pos_ = traj_length - 1;
    traj_begin_ = 0;
  }
  trajectory_[traj_pos_] = sf::Vertex(sf::Vector2f(pos_curr.x, pos_curr.y));
  shape.setPosition(trajectory_[traj_pos_].position -
                    sf::Vector2f(shape.getRadius(), shape.getRadius()));
};

void nbp::Body::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(&(trajectory_[traj_begin_]),
              (traj_pos_ >= traj_length) ? traj_length : traj_pos_,
              sf::LineStrip, states);
  target.draw(shape);
}