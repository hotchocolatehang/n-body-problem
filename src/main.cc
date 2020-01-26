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

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

#include "body.h"
#include "nbp_gui.h"
#include "physics/abstract_body.h"
#include "physics/gravity_simulation.h"

namespace nbp = n_body_problem;

constexpr char conf_file[] = "config/n-body-problem.conf";

// A period of time when only one mouse click can be given to the app. Made to
// prevent the effect when one human click represents as many inside the
// program.
constexpr std::chrono::duration<uint64_t, std::milli> min_time_between_clicks(
    200);  // = 200 ms

// A period of time in which one frame can be rendered to keep 30 FPS (30.32 ms)
// If a frame is rendered faster than the application sleeps the rest of the
// period.
constexpr std::chrono::duration<uint64_t, std::ratio<1L, 100000L>>
    frame_max_time(3032);

int main(int argc, char **argv) {
  if (argv[1] == NULL) {
    std::cerr << "Specify a simulation setup file!" << std::endl;
    return 1;
  }

  // Parameters setup
  int win_width = 800, win_height = 600;
  double click_precision = 10;

  std::fstream setup_file(conf_file);
  if (setup_file.good()) {
    std::string parameter;
    while (!setup_file.eof()) {
      setup_file >> parameter;

      if (parameter[0] == '#') {
        while (setup_file.get() != '\n')
          ;
      } else if (parameter == "window_height") {
        setup_file >> parameter >> win_height;
      } else if (parameter == "window_width") {
        setup_file >> parameter >> win_width;
      } else if (parameter == "trajectory_length") {
        setup_file >> parameter >> nbp::Body::traj_length;
      } else if (parameter == "click_precision") {
        setup_file >> parameter >> click_precision;
        click_precision *= click_precision;
      }
    };
  } else {
    std::cerr << "Error: Can't open configuration file '" << conf_file << "'. "
              << "Using default parameters.\n";
  }
  setup_file.close();

  // data setup
  std::string window_name = "N-body Problem - " + (std::string)argv[1];

  setup_file.open(argv[1]);
  if (!setup_file.good()) {
    std::cerr << "Error: Can't open simulation file '" << argv[1] << "'.\n";
    return 1;
  }

  size_t body_cnt;
  double delta_time, draw_scale;
  setup_file >> body_cnt >> delta_time >> draw_scale;

  // TODO: solve this bug. Cant process timescale > 100 because of:
  // nbp::Body::traj_length /= delta_time; // makes the length of trajectories
  // be same at different time scale (aka speed of simulation)

  std::vector<nbp::Body> bodies(body_cnt);
  for (size_t i = 0; i < body_cnt; i++) {
    nbp::vector2<double> _pos_curr, _velocity;
    long double _mass;
    std::string mass_str;
    double radius;
    unsigned _r, _g, _b;
    setup_file >> _pos_curr.x >> _pos_curr.y >> _velocity.x >> _velocity.y >>
        mass_str >> radius >> _r >> _g >> _b;
    char *end;
    _mass = strtold(mass_str.c_str(), &end);
    bodies[i] = nbp::Body(_pos_curr.x, win_height - _pos_curr.y, _velocity.x,
                          _velocity.y * -1, _mass, radius, _r, _g, _b);
  }
  setup_file.close();

  delta_time /= 30;

  // simulation
  nbp::GravitySimulation sim(delta_time);

  std::vector<nbp::AbstractBody *> bodies_ptrs(body_cnt);
  for (size_t i = 0; i < body_cnt; i++) bodies_ptrs[i] = &(bodies[i]);

  nbp::gui::CelestialObjectInfo obj_info;

  bool info_shown = false;
  bool paused = false;
  auto PauseSimulation = [&paused]() { paused = !paused; };
  auto IncreaseTimeScale = [&delta_time, &sim] {
    delta_time *= 2;
    sim.set_delta_time(delta_time);
  };
  auto DecreaseTimeScale = [&delta_time, &sim] {
    delta_time /= 2;
    sim.set_delta_time(delta_time);
  };

  // buttons:
  std::vector<std::pair<nbp::gui::Button, std::function<void()>>> buttons = {
      {nbp::gui::Button("res/buttons/decrease_time.png",
                        sf::Vector2f(0, win_height - 30)),
       DecreaseTimeScale},
      {nbp::gui::Button("res/buttons/play.png",
                        sf::Vector2f(30, win_height - 30)),
       PauseSimulation},
      {nbp::gui::Button("res/buttons/increase_time.png",
                        sf::Vector2f(60, win_height - 30)),
       IncreaseTimeScale}
      // TODO (not for MVP): show/hide trajectories, show/hide velocities
  };

  sf::RenderWindow main_win(sf::VideoMode(win_width, win_height), window_name);
  sf::Event win_event;
  auto last_click_time = std::chrono::system_clock::now();
  auto frame_start_time = std::chrono::system_clock::now();

  // main window cycle
  while (main_win.isOpen()) {
    while (main_win.pollEvent(win_event)) {
      if (win_event.type == sf::Event::Closed) {
        main_win.close();
        break;
      } else if (win_event.type == sf::Event::KeyPressed) {
        if (win_event.key.code == sf::Keyboard::Key::Space) PauseSimulation();
        if (win_event.key.code == sf::Keyboard::Key::Escape) {
          main_win.close();
          break;
        }
      }
    }

    // control handling
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      auto click_moment = std::chrono::system_clock::now();
      if (click_moment - last_click_time >= min_time_between_clicks) {
        last_click_time = click_moment;
        sf::Vector2i pressed = sf::Mouse::getPosition(main_win);
        for (auto &i : buttons) {
          auto &button_pos = i.first.img.getPosition();
          if (button_pos.x < pressed.x &&
              button_pos.x + i.first.width > pressed.x &&
              button_pos.y < pressed.y &&
              button_pos.y + i.first.width > pressed.y) {
            i.second();
            break;
          }
        }

        for (auto &body : bodies) {
          if (body.pos_curr.x - body.shape.getRadius() < pressed.x &&
              body.pos_curr.x + body.shape.getRadius() > pressed.x &&
              obj_info.getBodyPtr() != &body) {
            double distance_sq =
                fabs(pow((double)pressed.x - body.pos_curr.x, 2) +
                     pow((double)pressed.y - body.pos_curr.y, 2));
            if (distance_sq <= pow(body.shape.getRadius(), 2)) {
              obj_info = nbp::gui::CelestialObjectInfo(body);
              info_shown = true;
              break;
            }
          }
        }
      }
    }

    if (!paused) {
      sim.ApplyGravity(bodies_ptrs.begin(), bodies_ptrs.end());

      for (auto &body : bodies_ptrs) body->MoveToNextTimePoint();
    }
    main_win.clear();
    for (auto &body : bodies) main_win.draw(body);

    for (auto &button : buttons) main_win.draw(button.first);

    if (info_shown) {
      obj_info.UpdateInfo();
      main_win.draw(obj_info);
    }

    auto frame_calc_time = std::chrono::system_clock::now() - frame_start_time;
    if (frame_calc_time < frame_max_time)
      std::this_thread::sleep_for(frame_max_time - frame_calc_time);

    main_win.display();

    frame_start_time = std::chrono::system_clock::now();
  }

  return 0;
}