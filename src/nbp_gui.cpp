#include "nbp_gui.h"

#include <iostream>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <SFML/Graphics.hpp>

#include "abstract_body.h"
#include "body.h"
#include "gravity_simulation.h"

namespace nbp = n_body_problem;

nbp::gui::Button::Button(std::string path, sf::Vector2f pos)
{
  InitTexture(path);
  img.setTexture(*texture_);
  img.setPosition(pos);
  SetUp();
};

// void nbp::gui::Button::setTexture(std::string path)
// {
//   sf::Texture new_texture;
//   if (!new_texture.loadFromFile(path))
//     throw std::exception();
//   std::swap(texture_, new_texture);
//   SetUp();
// };

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
  
  titles[0].setString("Type");
  titles[1].setString("Class");
  titles[2].setString("Radius");
  titles[3].setString("Velocity");
  titles[4].setString("Gravity");

  position.x += 110;
  position.y -= offset * 5;

  type.setFillColor(sf::Color::White);
  type.setFont(*font_);
  type.setCharacterSize(text_size_);
  type.setPosition(position);
  class_of_obj = radius = velocity = gravity = type;
  position.y += offset;
  class_of_obj.setPosition(position);
  position.y += offset;
  radius.setPosition(position);
  position.y += offset;
  velocity.setPosition(position);
  position.y += offset;
  gravity.setPosition(position);

  Classify();
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

void nbp::gui::CelestialObjectInfo::Classify()
{
  // TODO: real classifyer
  std::string _type("Planet");
  std::string _class("Terra");
  float _radius = body_->shape.getRadius();
  float _gravity = nbp::GRAV_CONST * body_->mass / pow(_radius, 2);

  type.setString(_type);
  class_of_obj.setString(_class);
  radius.setString(boost::lexical_cast<std::string>(_radius));
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
  // target.draw(cursor_);
  for (auto& i : titles)
    target.draw(i);
  target.draw(type);
  target.draw(class_of_obj);
  target.draw(radius);
  target.draw(velocity);
  target.draw(gravity);
};
