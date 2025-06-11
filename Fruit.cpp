#include <SFML/Graphics.hpp>

#include "Fruit.h"
#include <iostream>

using namespace sfSnake;

const float Fruit::Radius = 5.f;

size_t Fruit::idCounter_ = 0;

Fruit::Fruit(sf::Vector2f position, FruitType type)
{
	position_ = position;
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);
	shape_.setOutlineColor(sf::Color::Yellow);
	shape_.setOutlineThickness(2.f);
	type_ = type;
	//shape_.setFillColor(sf::Color::Red);
	switch (type) {
	case FruitType::Black: shape_.setFillColor(sf::Color::Black); break;
	case FruitType::Brown: shape_.setFillColor(sf::Color(139, 69, 19)); break;
	case FruitType::Red:   shape_.setFillColor(sf::Color::Red); break;
	case FruitType::Blue:  shape_.setFillColor(sf::Color::Blue); break;
	case FruitType::Green: shape_.setFillColor(sf::Color::Green); break;
	}
	//texture_.loadFromFile("Images/fruit.png");
	//shape_.setTexture(&texture_);
	idCounter_++;
}

void Fruit::render(sf::RenderWindow& window)
{
	window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const
{
	return shape_.getGlobalBounds();
}