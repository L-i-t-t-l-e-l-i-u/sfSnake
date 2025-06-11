#include "RuleScreen.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "Game.h"
#include "SettingScreen.h"
#include <iostream>


using namespace sfSnake;



void RuleScreen::handleInput(sf::RenderWindow& window)  
{  
    // ·µ»Ø²Ëµ¥  
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {  
        Game::Screen = std::make_shared<MenuScreen>();  
    }  
}

void RuleScreen::update(sf::Time) {}

void RuleScreen::render(sf::RenderWindow& window)
{
    window.clear(SettingScreen::backgroundColor);

    font_.loadFromFile("Fonts/game_over.ttf");
    text_.setFont(font_);
    text_.setString(
        "\n"
        "\n\nThis is a snake game"
        "\n\nYou may use four arrays and WASD to control the snake, and you may also use the mouse left button."
        "\n\nYou die when you crash into the wall."
        "\n\nYou need to collect all the fruit on the Screen before the Timer goes out."
        "\n\nWhen you collect all, the timer resets."
        "\n\nEach time the timer resets, you are faster and need to collect more fruits."
		"\n\nYou can press Q to return to the menu."
    );
    text_.setCharacterSize(15);
    text_.setFillColor(sf::Color::Yellow);
    sf::FloatRect textBounds = text_.getLocalBounds();
    text_.setOrigin(textBounds.left + textBounds.width / 2,
        textBounds.top + textBounds.height / 2);
    text_.setPosition(static_cast<float>(Game::Width) / 2, static_cast<float>(Game::Height) / 2);
    window.draw(text_);
}