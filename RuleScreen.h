#pragma once
#pragma once
#include "Screen.h"
#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.h"

using namespace sfSnake;

class RuleScreen : public Screen
{
private:
    
    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

    sf::Font font_;
    sf::Text snakeText_;
    sf::Text text_;
};