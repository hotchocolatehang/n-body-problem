#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "abstract_body.h"
#include "body.h"
#include "gravity_simulation.h"

namespace nbp = n_body_problem;

size_t nbp::Body::traj_length = 100; // default hard-coded length of trajectory

constexpr char conf_file[] = "config/n-body-problem.conf";

int main(int argc, char **argv) 
{
  if (argv[1] == NULL) 
  {
    std::cout << "Specify a simulation setup file!" << std::endl;
    return 1;
  }

  // application setup
  int win_width = 800,
      win_height = 600;
  std::fstream setup_file(conf_file);
  if (setup_file.good())
  {
    std::string parameter;
    while (!setup_file.eof())
    {
      setup_file >> parameter;
      if (parameter[0] == '#') 
        while (setup_file.get() != '\n');
      else if (parameter == "window_height")
        setup_file >> parameter >> win_height;
      else if (parameter == "window_width")
        setup_file >> parameter >> win_width;
      else if (parameter == "trajectory_length")
        setup_file >> parameter >> nbp::Body::traj_length;
    };
  }
  else
  {
    std::cerr << "Error: Can't open configuration file '" << conf_file << "'. "
              << "Using default parameters.\n";
  }
  setup_file.close();

  // data setup
  std::string window_name = "N-body Problem - " + (std::string) argv[1];

  setup_file.open(argv[1]);
  if (!setup_file.good())
  {
    std::cerr << "Error: Can't open simulation file '" << argv[1] << "'.\n";
    return 1;
  }
  size_t body_cnt;
  double time_scale,
         draw_scale;
  setup_file >> body_cnt >> time_scale >> draw_scale;

  // TODO: solve this bug. Cant process timescale > 100 because of:
  // nbp::Body::traj_length /= time_scale; // makes the length of trajectories be same at different time scale (aka speed of simulation)

  std::vector<nbp::Body> bodies(body_cnt);
  for (size_t i = 0; i < body_cnt; i++) 
  {
    nbp::vector2<double> _pos_curr, 
                         _velocity;
    long double _mass;
    std::string mass_str;
    double radius;
    unsigned _r, _g, _b;
    setup_file >> _pos_curr.x >> _pos_curr.y >> _velocity.x >> _velocity.y
                >> mass_str >> radius >> _r >> _g >> _b;
    char *end;
    _mass = strtold(mass_str.c_str(), &end);
    bodies[i] = nbp::Body(_pos_curr.x, win_height - _pos_curr.y, _velocity.x, _velocity.y * -1, _mass, radius, _r, _g, _b);
  }
  setup_file.close();

  // simulation
  nbp::GravitySimulation sim(time_scale);

  std::vector<nbp::AbstractBody*> bodies_ptrs(body_cnt);
  for (size_t i = 0; i < body_cnt; i++)
    bodies_ptrs[i] = &(bodies[i]);

  sf::RenderWindow main_win (sf::VideoMode(win_width, win_height), window_name);
  // sf::ContextSettings settings;
  // settings.antialiasingLevel = 8;
  
  bool paused = false;
  sf::Event win_event;
  while (main_win.isOpen())
  {
    while (main_win.pollEvent(win_event))
    {
      if (win_event.type == sf::Event::Closed) {
        main_win.close();
        break;
      }
      if (win_event.type == sf::Event::KeyPressed) 
      {
        if (win_event.key.code == sf::Keyboard::Key::Space)
          paused = !paused;
        if (win_event.key.code == sf::Keyboard::Key::Escape) {
          main_win.close();
          break;
        }
      }
    }

    if (!paused) {
      sim.ApplyGravity(bodies_ptrs.begin(), bodies_ptrs.end());

      for (auto& body: bodies_ptrs)
        body -> MoveToNextTimePoint();

      main_win.clear();
      for (auto& body : bodies)
        main_win.draw(body);
      main_win.display();
    }
  }

  return 0;
}