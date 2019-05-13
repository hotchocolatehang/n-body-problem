#include "body.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace nbp = n_body_problem;

nbp::Body::Body()
  : AbstractBody(),
  pos_old({0, 0}),
  color (sf::Color(0, 0, 0)),
  traj_begin_ (0)
{

};

nbp::Body::Body(double x_pos, double y_pos, double x_vel, double y_vel, long _mass, double radius, unsigned _r, unsigned _g, unsigned _b) :
  AbstractBody(x_pos, y_pos, x_vel, y_vel, _mass),
  pos_old (x_pos, y_pos),
  color (_r, _g, _b), // color (sf::Color(_r, _g, _b)),
  shape (radius, 10),
  traj_begin_ (0)
{
  shape.setFillColor(color);
  trajectory_.push_back(sf::Vector2f(pos_curr.x, pos_curr.y));
};

nbp::Body::Body(vector2<double> _pos_curr, vector2<double> _velocity, long _mass, double radius, sf::Color _color) :
  AbstractBody(_pos_curr, _velocity, _mass),
  pos_old (_pos_curr),
  color (_color),
  shape(radius, 10),
  traj_begin_ (0)
{
  shape.setFillColor(color);
  trajectory_.push_back(sf::Vector2f(pos_curr.x, pos_curr.y));
};

nbp::Body::Body(Body const& other) :
  AbstractBody(other)
{
  color = other.color;
};

nbp::Body::~Body()
{

};

void nbp::Body::MoveToNextTimePoint()
{
  pos_old = pos_curr;
  AbstractBody::MoveToNextTimePoint();
  trajectory_.push_back(sf::Vertex(sf::Vector2f(pos_curr.x, pos_curr.y)));
  if (trajectory_.size() > traj_length)
    traj_begin_++;
  if (trajectory_.size() > 2000)
  {
    std::vector<sf::Vertex> new_traj(traj_length);
    for (size_t i = trajectory_.size() - traj_length; i < trajectory_.size(); i++)
      new_traj[i - (trajectory_.size() - traj_length)] = trajectory_[i];
    std::swap(trajectory_, new_traj);
    traj_begin_ = 0;
  }
  shape.setPosition(trajectory_.back().position - sf::Vector2f(shape.getRadius(), shape.getRadius()));
};

void nbp::Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(&(trajectory_[traj_begin_]),(trajectory_.size() >= traj_length)? traj_length : trajectory_.size(), sf::LineStrip, states);
  target.draw(shape);
}