#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
class SnakeNode
{
public:

	enum class Texturetype {Head, Body};
	SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0), 
		sf::Vector2f movVec = sf::Vector2f(0, 0),
		Texturetype type = Texturetype::Body
		);

	void setPosition(sf::Vector2f position);
	void setPosition(float x, float y);

	void move(double xOffset, double yOffset);
	static void set_defaultTexture(const sf::Texture& texture);
	static void set_HeadTexture(const sf::Texture& texture);

	const void render(sf::RenderWindow& window) const;

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	const sf::Vector2f& getMov() const{
		return moveVec_;
	}
	void SetMov(sf::Vector2f vec){
		moveVec_ = vec;
	}

	static const float Width;
	static const float Height;

private:
	sf::RectangleShape shape_;
	sf::Sprite sprite_;
	sf::Vector2f moveVec_;
	sf::Vector2f position_;
	static sf::Texture default_texture_;
	static sf::Texture head_texture_;
	Texturetype type_;
};
}

#endif