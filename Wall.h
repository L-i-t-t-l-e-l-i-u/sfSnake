
#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

namespace sfSnake{


class Wall {


	public:
		// in sf::Vector2f Size, the first parameter is width and the second is height
	Wall(sf::Vector2f position, sf::Vector2f Size);

	void render(sf::RenderWindow& window) {
		window.draw(wall_);
	}
	sf::FloatRect getBounds() const {
		return wall_.getGlobalBounds();
	}

	private:
		sf::RectangleShape wall_; // the wall is a rectangle shape
		sf::Vector2f position_; // position of the wall
		sf::Vector2f size_; // size of the wall:the first parameter is width and the second is height


};

}




#endif