#pragma once
#include "Screen.h"
#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.h"

using namespace sfSnake;

class SettingScreen : public Screen
{
public:
    SettingScreen();
    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

    static sf::Color backgroundColor;
    static sf::Color gridColor;
    static bool showGrid;

private:
    int bgColorIndex_ = 0;
    int gridColorIndex_ = 0;
    static const std::vector<sf::Color> colorOptions;
    bool lastKeyPressed3_ = false;
    bool lastKeyPressed2_ = false;
    bool lastKeyPressed1_ = false;

    sf::Font font_;
    sf::Text snakeText_;
    sf::Text text_;
};