#include <SFML/Graphics.hpp>

#include <memory>
#include "SettingScreen.h"
#include "RuleScreen.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

MenuScreen::MenuScreen()
{
	font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setFillColor(sf::Color::Yellow);
	text_.setString(
		"\n\n\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit"
		"\n\nPress E to go to Settings Page"
		"\n\nPress R to go to Rule Page"
	);

	snakeText_.setFont(font_);
	snakeText_.setString("Snake!");
	snakeText_.setFillColor(sf::Color::Green);
	snakeText_.setCharacterSize(64);
	snakeText_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(static_cast<float>(Game::Width) / 2, static_cast<float>(Game::Height) / 2);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
		snakeTextBounds.top + snakeTextBounds.height / 2);
	snakeText_.setPosition(static_cast<float>(Game::Width) / 2, static_cast<float>(Game::Height) / 4);
}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))//Start the game
		Game::Screen = std::make_shared<GameScreen>();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { //Go to settings page
		Game::Screen = std::make_shared<SettingScreen>();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { //Go to rule page
		Game::Screen = std::make_shared<RuleScreen>();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;

	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}
}

void MenuScreen::render(sf::RenderWindow& window)
{
	window.clear(SettingScreen::backgroundColor);
	window.draw(text_);
	window.draw(snakeText_);
}