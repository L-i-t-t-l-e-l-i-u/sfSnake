#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Game.h"
#include "GameScreen.h"
#include "GameOverScreen.h"
#include "SettingScreen.h"

using namespace sfSnake;

GameOverScreen::GameOverScreen(std::size_t score) : score_(static_cast<unsigned>(score))
{
    // 字体加载增加错误检查[2,9](@ref)
    if (!font_.loadFromFile("Fonts/game_over.ttf")) {
        throw std::runtime_error("Failed to load game over font");
    }

    text_.setFont(font_);
    text_.setString("Your score: " + std::to_string(score) + "!"
        "\n\nPress [SPACE] to retry"
        "\n\nPress [ESC] to quit");
    
    // 替换弃用的setColor为setFillColor[2,8](@ref)
    text_.setFillColor(sf::Color::Red);
    
    
    sf::FloatRect textBounds = text_.getLocalBounds();

	text_.setOrigin(textBounds.width / 2, textBounds.height / 2);

	text_.setPosition(Game::Width / 2.0f, Game::Height / 2.0f);
}

void GameOverScreen::handleInput(sf::RenderWindow& window)
{
    // 更新键盘枚举命名空间[6,10](@ref)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        Game::Screen = std::make_shared<GameScreen>();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
    }
}

// 保持update空实现（如需动画效果可在此添加）
void GameOverScreen::update(sf::Time delta)
{
    // 示例：可添加文字闪烁动画
    // static float accumulator = 0;
    // accumulator += delta.asSeconds();
    // text_.setFillColor(sf::Color::Red * (sin(accumulator * 5) * 0.5f + 0.5f));
}

void GameOverScreen::render(sf::RenderWindow& window)
{
    window.clear(SettingScreen::backgroundColor);
    window.draw(text_);
}