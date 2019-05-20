#include "abstract_body.h"

namespace nbp = n_body_problem;

nbp::AbstractBody::AbstractBody() :
  pos_curr (0, 0),
  pos_new (0, 0),
  velocity (0, 0),
  mass (1)
{

};

nbp::AbstractBody::AbstractBody(double x_pos, double y_pos, double x_vel, double y_vel, long double m) :
  pos_curr (x_pos, y_pos),
  pos_new (x_pos, y_pos),
  velocity (x_vel, y_vel),
  mass (m)
{
  
};

nbp::AbstractBody::AbstractBody(vector2<double> pos, vector2<double> vel, long double m) :
  pos_curr (pos),
  pos_new (pos),
  velocity (vel),
  mass (m)
{
  
};

nbp::AbstractBody::AbstractBody(AbstractBody const& other) 
{
  this -> mass = other.mass;
  this -> pos_curr = other.pos_curr;
  this -> pos_new = other.pos_new;
  this -> velocity = other.velocity;
}

nbp::AbstractBody::~AbstractBody()
{

};

void nbp::AbstractBody::ApplyForce(vector2<double> force, double timeshift)
{
  vector2<double> acceleraion(force.x / mass, force.y / mass);
  velocity.x = velocity.x + acceleraion.x * timeshift;
  velocity.y = velocity.y + acceleraion.y * timeshift;
  pos_new.x = pos_curr.x + velocity.x * timeshift;
  pos_new.y = pos_curr.y + velocity.y * timeshift;
};

void nbp::AbstractBody::MoveToNextTimePoint()
{
  pos_curr = pos_new;
};
