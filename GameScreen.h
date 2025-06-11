#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>
#include <random>
#include <memory>

#include "Screen.h"
#include "Snake.h"
#include "Fruit.h"
#include "Wall.h"

namespace sfSnake
{
class GameScreen : public Screen
{
public:
	GameScreen();
	void load_Texture();
	void load_Score();
	void load_Wall();
	void load_timer();
	void update_Scores();
	const size_t& get_diff() const {
		return timerresetcount_;
	}
	void update_Timer(bool cleared);
	static void drawGrid(sf::RenderWindow& window);

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	size_t timerresetcount_ = 0;
	void generateFruit();

private:
	/*Snake snake_;*/
	std::unique_ptr<Snake> snake_;
	std::vector<Fruit> fruit_;
	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;
	std::default_random_engine engine_;
	sf::Font font_;                
	sf::Text scoreText_;
	size_t score_;
	size_t lastScore_;
	sf::Clock timer_;
	sf::Text timerText_;
	std::vector<Wall> walls_;
};
}

#endif