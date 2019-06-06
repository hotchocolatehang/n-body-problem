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

#ifndef GRAVITY_SIMULATION_H
#define GRAVITY_SIMULATION_H

#include <cmath>
#include <vector>

#include "abstract_body.h"

namespace n_body_problem {

  constexpr double GRAV_CONST = 6.67408313131E-11;

  class GravitySimulation {
  public:
    GravitySimulation(double _delta_time);
    ~GravitySimulation();

    /**
     * Changes velocities of a given system with respect to gravity.
     * Works for the exact point in time and if there are no other
     * forces. If you just need vectorized forces use get_system_forces().
     */
    void ApplyGravity(std::vector<AbstractBody*>::iterator begin, std::vector<AbstractBody*>::iterator end);

    /**
     * Stores a gravitational forces to every body in the system to a given vector.
     */
    void get_system_forces(std::vector<AbstractBody*>::iterator body_begin,
        std::vector<AbstractBody*>::iterator body_end,
        std::vector<vector2<double>>::iterator forces_begin,
        std::vector<vector2<double>>::iterator forces_end);

    /**
     * Returns gravitational force for first_body made by both.
     * To get this force for the second body just multiply it by -1.
     */
    vector2<double> get_pair_force (AbstractBody *first_body, AbstractBody *second_body);

    void set_delta_time(double _delta_time);

  private:
    GravitySimulation(GravitySimulation const& other);
    GravitySimulation(GravitySimulation const&& other);

    double delta_time;
  };

} // namespace n_body_problem

#endif