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

#include "nbp_gui.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "physics/abstract_body.h"
#include "physics/gravity_simulation.h"

namespace nbp = n_body_problem;

nbp::gui::Button::Button(std::string path, sf::Vector2f pos)
{
  InitTexture(path);
  img.setTexture(*texture_);
  img.setPosition(pos);
  SetUp();
};

void nbp::gui::Button::setPosition(sf::Vector2f pos)
{
  img.setPosition(pos);
};

void nbp::gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(img);
};

void nbp::gui::Button::SetUp()
{
  img.setTexture(*texture_);
  img.setScale(sf::Vector2f(0.23, 0.23));
  width = texture_->getSize().x * 0.23;
  height = texture_->getSize().y * 0.23;
};

void nbp::gui::Button::InitTexture(std::string path) {
  if (texture_.use_count() == 0)
    texture_ = std::make_shared<sf::Texture> (sf::Texture());
  if (!texture_->loadFromFile(path))
    throw std::exception();
};

nbp::gui::CelestialObjectInfo::CelestialObjectInfo(nbp::Body& _body) :
  position({0, 10}),
  text_size_(16),
  body_(&_body)
{
  InitFont("res/OneSlot.ttf");

  double offset = (double)text_size_ + 7;
  for (auto& i : titles) {
    i.setFillColor(sf::Color::White);
    i.setFont(*font_);
    i.setCharacterSize(text_size_);
    i.setPosition(position);
    position.y += offset;
  }
  
  titles[0].setString("Radius");
  titles[1].setString("Velocity");
  titles[2].setString("Mass");
  titles[3].setString("Gravity");

  position.x += 110;
  position.y -= offset * titles.size();

  radius.setFillColor(sf::Color::White);
  radius.setFont(*font_);
  radius.setCharacterSize(text_size_);
  radius.setPosition(position);
  velocity = gravity = mass = radius;
  position.y += offset;
  velocity.setPosition(position);
  position.y += offset;
  mass.setPosition(position);
  position.y += offset;
  gravity.setPosition(position);

  DeduceInfo();
};

void nbp::gui::CelestialObjectInfo::UpdateInfo()
{
  float vel = sqrt( pow(body_->velocity.x, 2) + pow(body_->velocity.y, 2) );
  velocity.setString(boost::lexical_cast<std::string>(vel) + " px per s");
};

nbp::Body* nbp::gui::CelestialObjectInfo::getBodyPtr()
{
  return body_;
};

void nbp::gui::CelestialObjectInfo::DeduceInfo()
{
  float _radius = body_->shape.getRadius();
  float _mass = body_->mass;
  float _gravity = nbp::GRAV_CONST * body_->mass / pow(_radius, 2);

  radius.setString(boost::lexical_cast<std::string>(_radius) + " px");
  mass.setString(boost::lexical_cast<std::string>(_mass) + " kg");
  gravity.setString(boost::lexical_cast<std::string>(_gravity) + " g");
};

void nbp::gui::CelestialObjectInfo::InitFont(std::string name)
{
  if(font_.use_count() == 0)
    font_ = std::make_shared<sf::Font>(sf::Font());
  if (!font_->loadFromFile(name))
  {
    // TODO: make exceptions over all the project
    std::cerr << "Error: Can't open font file '" << name << "'\n";
    throw std::exception();
  };
};

void nbp::gui::CelestialObjectInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& i : titles)
    target.draw(i);
  target.draw(radius);
  target.draw(velocity);
  target.draw(mass);
  target.draw(gravity);
};
