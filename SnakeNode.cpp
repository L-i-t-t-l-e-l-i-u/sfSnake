#include <SFML/Graphics.hpp>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 5.f;
const float SnakeNode::Height = 5.f;
sf::Texture sfSnake::SnakeNode::default_texture_;
sf::Texture sfSnake::SnakeNode::head_texture_;

SnakeNode::SnakeNode(sf::Vector2f position, sf::Vector2f moveVec, Texturetype type)
: position_(position), type_(type), moveVec_(moveVec)
{
	if (type_ == Texturetype::Head) {
		shape_.setPosition(position_);
		/*shape_.setFillColor(sf::Color::Green);*/
		shape_.setSize(sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
		sprite_.setTexture(head_texture_);
		sprite_.setPosition(position_);
		sprite_.setScale(Width / head_texture_.getSize().x, Height / head_texture_.getSize().y);
	}else{

	    shape_.setPosition(position_);
	    /*shape_.setFillColor(sf::Color::Green);*/
	    shape_.setSize(sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
	    shape_.setOutlineColor(sf::Color::Black);
	    shape_.setOutlineThickness(0.f);
		shape_.setTexture(&default_texture_);
		sprite_.setTexture(default_texture_);
		sprite_.setPosition(position_);
		sprite_.setScale(Width / default_texture_.getSize().x, Height / default_texture_.getSize().y);
	}
}


void SnakeNode::setPosition(sf::Vector2f position)
{
	position_ = position;
	shape_.setPosition(position_);
	sprite_.setPosition(position_);
}

void SnakeNode::setPosition(float x, float y)
{
	position_.x = x;
	position_.y = y;
	shape_.setPosition(position_);
	sprite_.setPosition(position_);
}

void SnakeNode::move(double xOffset, double yOffset)
{
	position_.x += static_cast<float>(xOffset);
	position_.y += static_cast<float>(yOffset);
	shape_.setPosition(position_);
	sprite_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const
{
	return shape_.getGlobalBounds();
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}


void SnakeNode::set_defaultTexture(const sf::Texture& texture)
{
	default_texture_ = texture;
}

void SnakeNode::set_HeadTexture(const sf::Texture& texture)
{
	head_texture_ = texture;
}

const void SnakeNode::render(sf::RenderWindow& window) const
{

	float angle = 0.f;
	if (moveVec_.x == 0 && moveVec_.y == -1) angle = 0.f;         // Up
	else if (moveVec_.x == 1 && moveVec_.y == 0) angle = 90.f;    // Right
	else if (moveVec_.x == 0 && moveVec_.y == 1) angle = 180.f;   // Down
	else if (moveVec_.x == -1 && moveVec_.y == 0) angle = 270.f;  // Left
	else if (moveVec_.x != 0 || moveVec_.y != 0)
	angle = std::atan2(moveVec_.y, moveVec_.x) * 180.f / 3.1415926f + 90.f;
	if (type_ == Texturetype::Head) {
		sf::Sprite newhead;
		newhead.setOrigin(head_texture_.getSize().x / 2.f, head_texture_.getSize().y / 2.f);
		newhead.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
		newhead.setRotation(angle);
		newhead.setTexture(head_texture_);
		newhead.setScale(Width*5 / default_texture_.getSize().x, Height*5 / default_texture_.getSize().y);
		window.draw(newhead);
	}
	else
	{

		sf::Sprite newbody;

		newbody.setOrigin(default_texture_.getSize().x / 2.f, default_texture_.getSize().y / 2.f);
		newbody.setPosition(position_.x + SnakeNode::Width / 2.f, position_.y + SnakeNode::Height / 2.f);
		newbody.setRotation(angle);
		newbody.setTexture(default_texture_);
		newbody.setScale(Width * 5 / default_texture_.getSize().x, Height * 5 / default_texture_.getSize().y);
		window.draw(newbody);
	}
}

