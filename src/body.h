// Body class implementation

#ifndef BODY_H
#define BODY_H

#include <SFML/Graphics.hpp>
#include <string>

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

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Color color;
  sf::CircleShape shape;
  // TODO: change trajectory to std::array
  std::vector<sf::Vertex> trajectory_;
  int traj_begin_;
}; // class body_interface

} // namespace n_body_problem

#endif