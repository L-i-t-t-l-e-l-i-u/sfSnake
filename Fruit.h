#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
	class Fruit
	{
	public:
		enum class FruitType { Black, Brown, Red, Blue, Green };
		Fruit(sf::Vector2f position = sf::Vector2f(0, 0), FruitType type = FruitType::Green);




		void render(sf::RenderWindow& window);

		sf::FloatRect getBounds() const;

		const sf::Vector2f& getPosition() const {
			return position_;
		};
		const FruitType getFruitType() const {
			return type_;
		}

		const size_t getId() const {
			return idCounter_;
		};

		~Fruit() { idCounter_--; }

	private:
		sf::CircleShape shape_;
		sf::Vector2f position_;
		sf::Texture texture_;
		FruitType type_;

		static size_t idCounter_;

		static const float Radius;
	};
}

#endif