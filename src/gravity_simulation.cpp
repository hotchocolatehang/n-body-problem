#include "gravity_simulation.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#include "abstract_body.h"
namespace nbp = n_body_problem;

nbp::GravitySimulation::GravitySimulation(double _timeshift) :
  timeshift(_timeshift)
{

};

nbp::GravitySimulation::~GravitySimulation()
{

};

void nbp::GravitySimulation::ApplyGravity(std::vector<nbp::AbstractBody*> body_ptrs)
{
  // TODO: optimize everything. This is just a draft.
  std::vector<nbp::vector2<double>> force_sums( body_ptrs.size(), {0, 0});
  for (size_t i = 0; i < body_ptrs.size(); i++) 
  {
    for (size_t j = i + 1; j < body_ptrs.size(); j++) 
    {
      nbp::vector2<double> force = get_pair_force(body_ptrs[i], body_ptrs[j]);
      force_sums[i].x += force.x;
      force_sums[i].y += force.y;

      force_sums[j].x += force.x * -1;
      force_sums[j].y += force.y * -1;
    }
  }

  for (size_t i = 0; i < body_ptrs.size(); i++)
    body_ptrs[i] -> ApplyForce(force_sums[i], timeshift);
};

std::vector<nbp::vector2<double>> nbp::GravitySimulation::get_system_forces(std::vector<nbp::AbstractBody*> body_ptrs)
{
  std::vector<nbp::vector2<double>> force_sums( body_ptrs.size(), {0, 0});
  for (size_t i = 0; i < body_ptrs.size(); i++) 
  {
    for (size_t j = i + 1; j < body_ptrs.size(); j++) 
    {
      nbp::vector2<double> force = get_pair_force(body_ptrs[i], body_ptrs[j]);
      force_sums[i].x += force.x;
      force_sums[i].y += force.y;

      force_sums[j].x += force.x * -1;
      force_sums[j].y += force.y * -1;
    }
  }
  return force_sums;
};

nbp::vector2<double> nbp::GravitySimulation::get_pair_force (nbp::AbstractBody *first_body, nbp::AbstractBody *second_body)
{
  nbp::vector2<double> ret; // RVO?
  double distance = sqrt(
      pow(first_body -> pos_curr.x - second_body -> pos_curr.x, 2) +
      pow(first_body -> pos_curr.y - second_body -> pos_curr.y, 2)
    );
  double force = grav_const * ((first_body -> mass * second_body -> mass) / pow(distance, 2));
  nbp::vector2<double> raduis_vec = {second_body -> pos_curr.x - first_body -> pos_curr.x, second_body -> pos_curr.y - first_body -> pos_curr.y};
  ret = {force * raduis_vec.x, force * raduis_vec.y};
  return ret;
};