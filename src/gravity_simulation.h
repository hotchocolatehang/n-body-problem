#ifndef GRAVITY_SIMULATION_H
#define GRAVITY_SIMULATION_H

#include <cmath>
#include <cstdlib>
#include <vector>

#include "abstract_body.h"

namespace n_body_problem {
  class GravitySimulation {
  public:
    GravitySimulation(double _timeshift);
    ~GravitySimulation();

    /**
     * Changes velocities of a given system with respect to gravity.
     * Works for the exact point in time and if there are no other
     * forces. If you just need vectorized forces use get_system_forces().
     */
    void ApplyGravity(std::vector<AbstractBody*> body_ptrs);

    /**
     * Returns a vector of gravitational forces to every body in the system.
     */
    std::vector<vector2<double>> get_system_forces(std::vector<AbstractBody*> body_ptrs);

    /**
     * Returns gravitational force for first_body made by both.
     * To get this force for the second body just multiply it by -1.
     */
    vector2<double> get_pair_force (AbstractBody *first_body, AbstractBody *second_body);

  private:
    GravitySimulation(GravitySimulation const& other);
    GravitySimulation(GravitySimulation const&& other);

    double timeshift;
    const double grav_const = 6.674E-11;
  };

} // namespace n_body_problem

#endif