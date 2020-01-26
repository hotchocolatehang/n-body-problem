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

#include "abstract_body.h"

namespace nbp = n_body_problem;

nbp::AbstractBody::AbstractBody()
    : pos_curr(0, 0),
      pos_new(0, 0),
      velocity(0, 0),
      mass(1){

      };

nbp::AbstractBody::AbstractBody(double x_pos, double y_pos, double x_vel,
                                double y_vel, long double m)
    : pos_curr(x_pos, y_pos),
      pos_new(x_pos, y_pos),
      velocity(x_vel, y_vel),
      mass(m){

      };

nbp::AbstractBody::AbstractBody(vector2<double> pos, vector2<double> vel,
                                long double m)
    : pos_curr(pos),
      pos_new(pos),
      velocity(vel),
      mass(m){

      };

nbp::AbstractBody::AbstractBody(AbstractBody const& other) {
  this->mass = other.mass;
  this->pos_curr = other.pos_curr;
  this->pos_new = other.pos_new;
  this->velocity = other.velocity;
}

nbp::AbstractBody::~AbstractBody(){

};

void nbp::AbstractBody::ApplyForce(vector2<double> force, double timeshift) {
  vector2<double> acceleraion(force.x / mass, force.y / mass);
  velocity.x = velocity.x + acceleraion.x * timeshift;
  velocity.y = velocity.y + acceleraion.y * timeshift;
  pos_new.x = pos_curr.x + velocity.x * timeshift;
  pos_new.y = pos_curr.y + velocity.y * timeshift;
};

void nbp::AbstractBody::MoveToNextTimePoint() { pos_curr = pos_new; };
