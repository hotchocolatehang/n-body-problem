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

#ifndef N_BODY_PROBLEM_PHYSICS_ABSTRACT_BODY_H_
#define N_BODY_PROBLEM_PHYSICS_ABSTRACT_BODY_H_

namespace n_body_problem {

template<typename T>
struct vector2 {
	vector2() = default;
	vector2(T _x, T _y) : x(_x), y(_y) {};
	T x,
		y;
}; // struct vector2<>

struct AbstractBody {
	AbstractBody();
  AbstractBody(double x_pos, double y_pos, double x_vel, double y_vel, long double m);
	AbstractBody(vector2<double> pos, vector2<double> vel, long double m);
	AbstractBody(AbstractBody const& other);
  virtual ~AbstractBody();
	virtual void ApplyForce(vector2<double> force, double timeshift);
	virtual void MoveToNextTimePoint();

	vector2<double> pos_curr;
	vector2<double> pos_new;
	vector2<double> velocity;
	long double mass;
}; // struct AbstractBody

} // namespace n_body_problem

#endif // N_BODY_PROBLEM_PHYSICS_ABSTRACT_BODY_H_