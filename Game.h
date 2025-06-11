#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>

#include "Screen.h"

namespace sfSnake
{
class Game
{
public:
	Game();

	void run();

	const sf::Time& getTPF() const {//getter
		return TimePerFrame;
	}

	static void setTPF(sf::Time re) {//setter
		TimePerFrame = re;
	}


	void handleInput();
	void update(sf::Time delta);
	void render();

	static const int Width = 1280;
	static const int Height = 960;

	static std::shared_ptr<Screen> Screen;//创建一个静态的Screen指针

private:
	sf::RenderWindow window_;
	sf::Music bgMusic_;
	
	static sf::Time TimePerFrame;
};
}


#endif