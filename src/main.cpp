#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "abstract_body.h"
#include "body.h"
#include "gravity_simulation.h"
#include "nbp_gui.h"

namespace nbp = n_body_problem;

size_t nbp::Body::traj_length = 100; // default hard-coded length of trajectory

constexpr char conf_file[] = "config/n-body-problem.conf";

int main(int argc, char **argv) 
{
  if (argv[1] == NULL) 
  {
    std::cerr << "Specify a simulation setup file!" << std::endl;
    return 1;
  }

  // application setup
  int win_width = 800,
      win_height = 600;
  double click_precision = 10;
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
      else if (parameter == "click_precision") {
        setup_file >> parameter >> click_precision;
        click_precision *= click_precision;
      }
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
  double delta_time,
         draw_scale;
  setup_file >> body_cnt >> delta_time >> draw_scale;

  // TODO: solve this bug. Cant process timescale > 100 because of:
  // nbp::Body::traj_length /= delta_time; // makes the length of trajectories be same at different time scale (aka speed of simulation)

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
  nbp::GravitySimulation sim(delta_time);

  std::vector<nbp::AbstractBody*> bodies_ptrs(body_cnt);
  for (size_t i = 0; i < body_cnt; i++)
    bodies_ptrs[i] = &(bodies[i]);

  // buttons:
  std::vector<std::pair<nbp::gui::Button, std::function<void()>>> buttons(1);
  buttons[0].first.setTexture("res/buttons/pause.png");
  buttons[0].first.setPosition(sf::Vector2f(0, win_height - 30));
  
  nbp::gui::CelestialObjectInfo obj_info;

  bool obj_info_showed = false;
  bool paused = false;
  auto PauseSimulation = [&paused]() {
    paused = !paused;
  };

  buttons[0].second = PauseSimulation;

  sf::RenderWindow main_win (sf::VideoMode(win_width, win_height), window_name);
  // sf::ContextSettings settings;
  // settings.antialiasingLevel = 8;
  sf::Event win_event;
  while (main_win.isOpen())
  {
    while (main_win.pollEvent(win_event))
    {
      if (win_event.type == sf::Event::Closed) {
        main_win.close();
        break;
      }
      else if (win_event.type == sf::Event::KeyPressed) 
      {
        if (win_event.key.code == sf::Keyboard::Key::Space)
          PauseSimulation();
        if (win_event.key.code == sf::Keyboard::Key::Escape) 
        {
          main_win.close();
          break;
        }
      }
    }

    // control handling
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      sf::Vector2i pressed = sf::Mouse::getPosition(main_win);
      for (auto& i : buttons) {
        auto& button_pos = i.first.img.getPosition();
        if (button_pos.x < pressed.x && button_pos.x + i.first.width > pressed.x
        &&  button_pos.y < pressed.y && button_pos.y + i.first.width > pressed.y
        /*&& set time or smth to make buttons work correct because single human click is actually multiple clicks*/) {
          i.second();
          break;
        }
      }

      for (auto& body : bodies) {
            if (body.pos_curr.x - body.shape.getRadius() < pressed.x
            &&  body.pos_curr.x + body.shape.getRadius() > pressed.x
            && obj_info.getBodyPtr() != &body) 
            {
              double distance_sq = fabs(
                      pow((double)pressed.x - body.pos_curr.x, 2) +
                      pow((double)pressed.y - body.pos_curr.y, 2));
              if (distance_sq <= pow(body.shape.getRadius(), 2)) 
              {
                obj_info = nbp::gui::CelestialObjectInfo(body);
                obj_info_showed = true;
                break;
              }
            }
          }
    }

    if (!paused) {
      sim.ApplyGravity(bodies_ptrs.begin(), bodies_ptrs.end());

      for (auto& body: bodies_ptrs)
        body -> MoveToNextTimePoint();
    }
    main_win.clear();
    for (auto& body : bodies)
      main_win.draw(body);

    for (auto& i : buttons)
      main_win.draw(i.first);

    if (obj_info_showed) {
      obj_info.UpdateInfo(delta_time);
      main_win.draw(obj_info);
    }

    main_win.display();
  }

  return 0;
}