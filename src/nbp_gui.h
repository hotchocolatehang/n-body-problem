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

#ifndef N_BODY_PROBLEM_NBP_GUI_H_
#define N_BODY_PROBLEM_NBP_GUI_H_

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <boost/lexical_cast.hpp>

#include "body.h"
#include "physics/abstract_body.h"
#include "physics/gravity_simulation.h"

namespace n_body_problem {
namespace gui {

class Button : public sf::Drawable {
 public:
  Button() = default;
  Button(std::string path, sf::Vector2f pos);
  void setPosition(sf::Vector2f pos);

  unsigned height, width;
  sf::Sprite img;

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void SetUp();
  void InitTexture(std::string path);

  std::shared_ptr<sf::Texture> texture_;
};

class CelestialObjectInfo : public sf::Drawable {
 public:
  CelestialObjectInfo() = default;
  CelestialObjectInfo(Body& _body);

  void UpdateInfo();
  Body* getBodyPtr();

  sf::Text radius, velocity, mass, gravity;

 private:
  void InitFont(std::string name);
  void DeduceInfo();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::array<sf::Text, 4> titles;
  sf::Vector2f position;
  std::shared_ptr<sf::Font> font_;
  unsigned text_size_;
  Body* body_;
};
};  // namespace gui
};  // namespace n_body_problem

#endif  // N_BODY_PROBLEM_NBP_GUI_H_