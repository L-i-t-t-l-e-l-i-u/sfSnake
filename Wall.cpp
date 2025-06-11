#include <SFML/Graphics.hpp>

#include "Wall.h"

using namespace sfSnake;

Wall::Wall(sf::Vector2f position, sf::Vector2f Size) {
	position_ = position;
	size_ = Size;
	wall_.setPosition(position);
	wall_.setSize(Size);
	wall_.setFillColor(sf::Color::Red);
}


