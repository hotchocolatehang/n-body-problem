#ifndef NBP_GUI_H
#define NBP_GUI_H

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

namespace n_body_problem {
  namespace gui {

class Button : public sf::Drawable {
public:
  Button() = default;
  Button(std::string path, sf::Vector2f pos);
  void setTexture(std::string path);
  void setPosition(sf::Vector2f pos);

  unsigned height,
           width;
  sf::Sprite img;
private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void SetUp();

  sf::Texture texture_;
};

class CelestialObjectInfo : public sf::Drawable {
public:
  CelestialObjectInfo() = default;
  CelestialObjectInfo(Body& _body);

  void UpdateInfo(double delta_time);
  Body* getBodyPtr();

  sf::Text type,
           class_of_obj,
           radius,
           velocity,
           gravity;
  
private:
  void InitFont(std::string name);
  void Classify();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::array<sf::Text, 5> titles;
  sf::Vector2f position;
  std::shared_ptr<sf::Font> font_;
  unsigned text_size_;
  Body* body_;
};
  }; // namespace gui
}; // namespace n_body_problem

#endif