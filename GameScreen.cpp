#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>



#include "SnakeNode.h"
#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"
#include "Fruit.h"
#include "SettingScreen.h"



using namespace sfSnake;

GameScreen::GameScreen()
{
	load_Texture();
	load_Score();
	load_Wall();
	load_timer();
	engine_.seed(static_cast<unsigned>(time(NULL)));
	timerresetcount_ = 0;
	score_ = 0;
	lastScore_ = 0;

	snake_ = std::make_unique<Snake>();
}


void GameScreen::load_Texture(){

	sf::Texture defaultTexture;//Snake body texture
	sf::Texture headTexture;//Snake head texture
	defaultTexture.loadFromFile("Images/body.png");
	headTexture.loadFromFile("Images/head.png");
	SnakeNode::set_defaultTexture(defaultTexture);
	SnakeNode::set_HeadTexture(headTexture);
}

void GameScreen::load_Score() {//init for score text
	font_.loadFromFile("Fonts/game_over.ttf");
	scoreText_.setFont(font_);
	scoreText_.setCharacterSize(24);
	scoreText_.setFillColor(sf::Color::Green);
	scoreText_.setPosition(10.f, 10.f);
	score_ = 0;
	scoreText_.setString("Score: 0");

}

void GameScreen::load_Wall() {//load wall config from file

	std::ifstream fin("wall.txt");
	float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
	while (fin >> x >> y >> width >> height) {
		sf::Vector2f wallPosition(x, y);
		sf::Vector2f wallSize(width, height);
		walls_.emplace_back(Wall(wallPosition, wallSize));
	}
	
}

void GameScreen::load_timer()//initialize timer text
{
	font_.loadFromFile("Fonts/game_over.ttf");
	timerText_.setFont(font_);
	timerText_.setCharacterSize(24);
	timerText_.setFillColor(sf::Color::Green);
	timerText_.setPosition(10.f, 40.f);
	timerText_.setString("Time: 0");
	timer_.restart();
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	snake_->handleInput(window);
}

void GameScreen::update(sf::Time delta)
{

	if (fruit_.size() == 0) {
		for (int i = 0; i <= 3 + timerresetcount_; i++) {
			generateFruit();

		}
		update_Timer(true);
	}


	snake_->update(delta);
	snake_->checkFruitCollisions(fruit_);
	snake_->checkWallCollisions(walls_);
	update_Scores();
	update_Timer(false);




	if (snake_->endgame())
		Game::Screen = std::make_shared<GameOverScreen>(snake_->getSize());
	//All the GameScreen data, Snake data, SnakeNode data, Fruit data,
	//Wall data will be safely destroyed when GameScreen is destroyed, and we need to pass the snake size to GameOverScreen


}


void GameScreen::update_Scores() {
	size_t currentSize = snake_->getSize();
	//std::cout << "Current Size: " << currentSize << ", Last Size: " << lastSize << std::endl;

	if (currentSize > lastScore_) {
		score_  = snake_->getNodes().size();
		scoreText_.setString("Score: " + std::to_string(score_));
		lastScore_ = currentSize;
	}

}


void GameScreen::update_Timer(bool cleared) {  
    if (cleared) {  
        timer_.restart();  
        timerresetcount_++;  
		float fps = std::min<float>(20.f + timerresetcount_ * 10,60.f);

        Game::setTPF(sf::seconds(1/fps)); 
    }  
    sf::Time elapsed = timer_.getElapsedTime();  
    int seconds = 40 - static_cast<int>(elapsed.asSeconds());  
    timerText_.setString("Time: " + std::to_string(seconds));  
    if (seconds <= 0) {  
        snake_->die();  
    }  
    else if (seconds <= 10) {  
        timerText_.setFillColor(sf::Color::Red);  
    }  
    else {  
        timerText_.setFillColor(sf::Color::Green);  
    }  
}

void GameScreen::drawGrid(sf::RenderWindow& window) {
	int spacing = static_cast<int>(SnakeNode::Width * 5);
	sf::Color gridColor = SettingScreen::gridColor;
	for (int x = 0; x < Game::Width; x += spacing) {
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(static_cast<float>(x), 0), gridColor),
			sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(Game::Height)), gridColor)
		};
		window.draw(line, 2, sf::Lines);
	}
	for (int y = 0; y < Game::Height; y += spacing) {
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(0, static_cast<float>(y)), gridColor),
			sf::Vertex(sf::Vector2f(static_cast<float>(Game::Width), static_cast<float>(y)), gridColor)
		};
		window.draw(line, 2, sf::Lines);
	}
}

void GameScreen::render(sf::RenderWindow& window)
{
	/*window.draw(backgroundSprite_);*/
	window.clear(SettingScreen::backgroundColor);	
	if (SettingScreen::showGrid) {
		drawGrid(window);
	}
	snake_->render(window);

	for (auto& fruit : fruit_)
		fruit.render(window);

	window.draw(scoreText_);
	window.draw(timerText_);
	for (auto& wall : walls_) {
		wall.render(window);
	}

}

void GameScreen::generateFruit()
{

	static std::uniform_int_distribution<int> xDistribution(0, static_cast<int>(Game::Width - SnakeNode::Width));
	static std::uniform_int_distribution<int> yDistribution(0, static_cast<int>(Game::Height - SnakeNode::Height));
	static std::uniform_real_distribution<float> typeDist(0.f, 1.f);




	while (true) {
		float r = typeDist(engine_);
		Fruit::FruitType type;
		if (r < 0.125f) type = Fruit::FruitType::Black; // 12.5%
		else if (r < 0.25f) type = Fruit::FruitType::Brown; // 12.5%
		else if (r < 0.5f) type = Fruit::FruitType::Red; // 25%
		else if (r < 0.75f) type = Fruit::FruitType::Blue; // 25%
		else type = Fruit::FruitType::Green; // 25%
		Fruit newFruit(sf::Vector2f(static_cast<float>(xDistribution(engine_)), static_cast<float>(yDistribution(engine_))),type);
		bool isValid = true;
		for (auto& snakenode : snake_->getNodes()) {//Make Sure the fruit does not overlap with the snake
			if (newFruit.getPosition() == snakenode.getPosition()) {
				isValid = false;
				break;
			}
		}
		for (auto& fruit : fruit_) {//Make Sure the fruit does not overlap with other fruits
			if (newFruit.getPosition() == fruit.getPosition()) {
				isValid = false;
				break;
			}
		}
		for (auto& wall : walls_) {//Make Sure the fruit does not overlap with walls
			if (newFruit.getBounds().intersects(wall.getBounds())) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			fruit_.push_back(newFruit);
			break;
		}
	}
}

