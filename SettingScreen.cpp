#include "SettingScreen.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "Game.h"
#include <iostream>


using namespace sfSnake;
const std::vector<sf::Color> SettingScreen::colorOptions = {
    sf::Color::Black,
    sf::Color(139, 69, 19), // 棕色
    sf::Color::White
};

sf::Color SettingScreen::backgroundColor = sf::Color::Black;
sf::Color SettingScreen::gridColor = sf::Color::White;
bool SettingScreen::showGrid = true;

SettingScreen::SettingScreen() {}

void SettingScreen::handleInput(sf::RenderWindow& window)
{
    // 背景色切换
    bool bgcolorKeyNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
    if (bgcolorKeyNow && !lastKeyPressed1_) {
        bgColorIndex_ = (bgColorIndex_ + 1) % colorOptions.size();
        backgroundColor = colorOptions[bgColorIndex_];
    }
    // 网格色切换

    bool gridcolorKeyNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
    if (gridcolorKeyNow && !lastKeyPressed2_) {
        gridColorIndex_ = (gridColorIndex_ + 1) % colorOptions.size();
        gridColor = colorOptions[gridColorIndex_];
    }
    // 网格开关
    bool gridKeyNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
    if (gridKeyNow && !lastKeyPressed3_) {
        showGrid = !showGrid;
        std::cout << showGrid << std::endl;
    }
	// 更新按键状态
    lastKeyPressed3_ = gridKeyNow;
    lastKeyPressed2_ = gridcolorKeyNow;
	lastKeyPressed1_ = bgcolorKeyNow;
    // 返回菜单
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        Game::Screen = std::make_shared<MenuScreen>();
    }
}

void SettingScreen::update(sf::Time) {}

void SettingScreen::render(sf::RenderWindow& window)
{
    window.clear(backgroundColor);
    if (SettingScreen::showGrid) {
        GameScreen::drawGrid(window);
    }

    font_.loadFromFile("Fonts/game_over.ttf");
    text_.setFont(font_);
    text_.setString(
        "\n\n\n\nPress 1 to change background color"
        "\n\nPress 2 to change grid color"
		"\n\nPress 3 to toggle grid visibility"
		"\n\nPress Q to return to menu"
    );
    text_.setCharacterSize(24);
    text_.setFillColor(sf::Color::Yellow);
    sf::FloatRect textBounds = text_.getLocalBounds();
    text_.setOrigin(textBounds.left + textBounds.width / 2,
        textBounds.top + textBounds.height / 2);
    text_.setPosition(static_cast<float>(Game::Width) / 2, static_cast<float>(Game::Height) / 2);
	window.draw(text_);
}