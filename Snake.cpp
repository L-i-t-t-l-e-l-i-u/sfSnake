#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"
#include "Wall.h"
#include "GameScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

Snake::Snake() : direction_(Direction::Right), die_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.aiff");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);
}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		SnakeNode::Texturetype textureType = (i == 0) ? SnakeNode::Texturetype::Head : SnakeNode::Texturetype::Body;
		nodes_.push_back(SnakeNode(sf::Vector2f(
			Game::Width / 2 - SnakeNode::Width / 2,
			Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)),
			sf::Vector2f(1.f,0.f),
			textureType));
		
	}
}

void Snake::handleInput(sf::RenderWindow& window)
{
	//处理键盘输入
	//不允许蛇头直接掉头
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction_ != Direction::Down)
		direction_ = Direction::Up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction_ != Direction::Up)
		direction_ = Direction::Down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction_ != Direction::Right)
		direction_ = Direction::Left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction_ != Direction::Left)
		direction_ = Direction::Right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction_ != Direction::Down)
		direction_ = Direction::Up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction_ != Direction::Up)
		direction_ = Direction::Down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction_ != Direction::Right)
		direction_ = Direction::Left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction_ != Direction::Left)
		direction_ = Direction::Right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		die();
	}

	// 鼠标左键按下时，计算方向向量
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// 获取鼠标在窗口中的像素坐标
		sf::Vector2i mousePixel = sf::Mouse::getPosition(window); 
		// 将像素坐标转换为世界坐标,允许自由调整窗口的大小和缩放
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);


		// 获取蛇头中心坐标
		sf::Vector2f headPos = nodes_[0].getPosition();
		// 计算方向向量
		sf::Vector2f dir(mouseWorld.x - headPos.x, mouseWorld.y - headPos.y);

		// 归一化为单位向量
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		float check = dir.x / len * moveVec_.x + dir.y /len * moveVec_.y;//两个模为1的向量内积，范围[-1,1]，越接近1，方向越相同，越接近-1，方向越相反
		if (check > -0.7 && len > SnakeNode::Width) {// 确保方向向量与当前移动方向不能几乎完全相反，且鼠标距蛇头距离大于蛇节点宽度
			customDirection_ = sf::Vector2f(dir.x / len, dir.y / len);
			useCustomDirection_ = true;// 设置为使用自定义方向
		}
	}
}

void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
	auto toRemove = fruits.end();
	Fruit::FruitType eatType = Fruit::FruitType::Green;

	for (auto& it = fruits.begin(); it != fruits.end(); ++it)
	{
		if (it->getBounds().intersects(nodes_[0].getBounds()))
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		int growLen = 0;
		eatType = toRemove->getFruitType();
		switch (eatType) {
		case Fruit::FruitType::Red: growLen = 3; break;
		case Fruit::FruitType::Blue: growLen = 2; break;
		case Fruit::FruitType::Green: growLen = 1; break;
		default: growLen = 0; break; // Black/Brown
		}
		for (int i = 0; i < growLen; ++i){ 
			grow(); 
			//std::cout << "growed" << std::endl;
		}
		fruits.erase(toRemove);
	}
}

void Snake::grow()
{
	sf::Vector2f newPosition = nodes_[nodes_.size() - 1].getPosition() * 2.f - nodes_[nodes_.size() - 2].getPosition();

	nodes_.push_back(SnakeNode(newPosition,
		nodes_[nodes_.size() - 1].getMov()));

}

unsigned Snake::getSize() const
{
	return static_cast<unsigned>(nodes_.size());
}

bool Snake::endgame() const
{
	return die_;
}


void Snake::die() {
	dieSound_.play();
	sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
	die_ = true;
}

void Snake::checkSelfCollisions()
{
	SnakeNode& headNode = nodes_[0];

	for (decltype(nodes_.size()) i = 2; i < nodes_.size(); ++i)
	{
		if (headNode.getBounds().intersects(nodes_[i].getBounds()))
		{
			die();
		}
	}
}

void Snake::checkEdgeCollisions()
{
	SnakeNode& headNode = nodes_[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::checkWallCollisions(std::vector<Wall> walls)
{
	SnakeNode& headNode = nodes_[0];
	for (auto& it : walls)
	{
		if (it.getBounds().intersects(headNode.getBounds()))
		{
			die();
		}
			
	}
}

void Snake::move()
{
	//记录上一步每个节点的位置和移动向量，方便后续节点跟随
	std::vector<sf::Vector2f> prevPositions(nodes_.size());
	std::vector<sf::Vector2f> prevMoveVecs(nodes_.size());
	for (size_t i = 0; i < nodes_.size(); ++i) {
		prevPositions[i] = nodes_[i].getPosition();
		prevMoveVecs[i] = nodes_[i].getMov();
	}

	//如果需要对齐到标准方向
	if (!useCustomDirection_ && needtoreset_) {
		static const sf::Vector2f directions[] = 
		{
           sf::Vector2f(0, -1), // Up
           sf::Vector2f(0, 1),  // Down
           sf::Vector2f(-1, 0), // Left
           sf::Vector2f(1, 0)   // Right
		};
		float maxDot = -2.0f;
		int best = 3; 
		//找到最接近moveVec_的标准方向
		for (int i = 0; i < 4; ++i) {
			float dot = moveVec_.x * directions[i].x + moveVec_.y * directions[i].y;
			if (dot > maxDot) {
				maxDot = dot;
				best = i;
			}
		}
		// 设置direction_为最近的标准方向
		switch (best) {
		case 0: direction_ = Direction::Up; break;
		case 1: direction_ = Direction::Down; break;
		case 2: direction_ = Direction::Left; break;
		case 3: direction_ = Direction::Right; break;
		}
		// 吸附后，moveVec_也对齐到标准方向
		moveVec_ = directions[best];
		// 重置标志
		needtoreset_ = false;
	}

	// 如果使用自定义方向，直接使用customDirection_，并重置标志
	if(useCustomDirection_){
		moveVec_ = customDirection_;
		useCustomDirection_ = false;
		needtoreset_ = true;

	}

	else{
		// 如果没有自定义方向，使用键盘逻辑
		switch (direction_) {
		case Direction::Up:
			moveVec_ = sf::Vector2f(0, -1);
			break;
		case Direction::Down:
			moveVec_ = sf::Vector2f(0, 1);
			break;
		case Direction::Left:
			moveVec_ = sf::Vector2f(-1, 0);
			break;
		case Direction::Right:
			moveVec_ = sf::Vector2f(1, 0);
			break;
		}
	}

	//更新头结点位置和移动向量
	nodes_[0].move(moveVec_.x * SnakeNode::Width * 1.5 , moveVec_.y * SnakeNode::Height * 1.5);
	nodes_[0].SetMov(moveVec_);

	//更新其他节点位置和移动向量
	for (size_t i = 1; i < nodes_.size();++i)
	{
		nodes_[i].setPosition(prevPositions[i - 1]);
		nodes_[i].SetMov(prevMoveVecs[i - 1]);
	}
}


void Snake::render(sf::RenderWindow& window)
{
	for (auto& node : nodes_)
		node.render(window);
}
