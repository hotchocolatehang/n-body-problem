// Body class interface

#ifndef ABSTRACT_BODY_H
#define ABSTRACT_BODY_H

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
  // virtual void Render(double scale) const = 0;

	vector2<double> pos_curr;
	vector2<double> pos_new;
	vector2<double> velocity;
	long double mass;
}; // struct AbstractBody

} // namespace n_body_problem

#endif