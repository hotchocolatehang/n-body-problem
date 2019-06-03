#include "gravity_simulation.h"

#include <cmath>

#include <vector>

#include "abstract_body.h"
namespace nbp = n_body_problem;

nbp::GravitySimulation::GravitySimulation(double _delta_time) :
  delta_time(_delta_time)
{

};

nbp::GravitySimulation::~GravitySimulation()
{

};

void nbp::GravitySimulation::ApplyGravity(std::vector<AbstractBody*>::iterator begin, std::vector<AbstractBody*>::iterator end)
{
  std::vector<nbp::vector2<double>> force_sums(end - begin, {0, 0});
  get_system_forces(begin, end, force_sums.begin(), force_sums.end());

  for (auto i = begin; i != end; i++)
    (*i)->ApplyForce(force_sums[i - begin], delta_time);
};

void nbp::GravitySimulation::get_system_forces(std::vector<AbstractBody*>::iterator body_begin,
    std::vector<AbstractBody*>::iterator body_end,
    std::vector<vector2<double>>::iterator forces_begin,
    std::vector<vector2<double>>::iterator forces_end)
{
  // TODO: handle errors
  nbp::vector2<double> force;
  for (auto i = body_begin; i != body_end; i++)
    for (auto j = i + 1; j != body_end; j++)
    {
      force = get_pair_force(*i, *j);

      (*(forces_begin + (i - body_begin))).x += force.x;
      (*(forces_begin + (i - body_begin))).y += force.y;

      (*(forces_begin + (j - i))).x += force.x * -1;
      (*(forces_begin + (j - i))).y += force.y * -1;
    }
};

nbp::vector2<double> nbp::GravitySimulation::get_pair_force (nbp::AbstractBody *first_body, nbp::AbstractBody *second_body)
{
  // TODO: optimize if possible
  nbp::vector2<double> ret;
  double distance = sqrt(
      pow(first_body -> pos_curr.x - second_body -> pos_curr.x, 2) +
      pow(first_body -> pos_curr.y - second_body -> pos_curr.y, 2)
    );
  double force = GRAV_CONST * ((first_body -> mass * second_body -> mass) / pow(distance, 2));
  nbp::vector2<double> raduis_vec = {second_body -> pos_curr.x - first_body -> pos_curr.x, second_body -> pos_curr.y - first_body -> pos_curr.y};
  ret = {force * raduis_vec.x, force * raduis_vec.y};
  return ret;
};

void nbp::GravitySimulation::set_delta_time(double _delta_time)
{
  delta_time = _delta_time;
}