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
	//�����������
	//��������ͷֱ�ӵ�ͷ
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

	// ����������ʱ�����㷽������
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// ��ȡ����ڴ����е���������
		sf::Vector2i mousePixel = sf::Mouse::getPosition(window); 
		// ����������ת��Ϊ��������,�������ɵ������ڵĴ�С������
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);


		// ��ȡ��ͷ��������
		sf::Vector2f headPos = nodes_[0].getPosition();
		// ���㷽������
		sf::Vector2f dir(mouseWorld.x - headPos.x, mouseWorld.y - headPos.y);

		// ��һ��Ϊ��λ����
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		float check = dir.x / len * moveVec_.x + dir.y /len * moveVec_.y;//����ģΪ1�������ڻ�����Χ[-1,1]��Խ�ӽ�1������Խ��ͬ��Խ�ӽ�-1������Խ�෴
		if (check > -0.7 && len > SnakeNode::Width) {// ȷ�����������뵱ǰ�ƶ������ܼ�����ȫ�෴����������ͷ��������߽ڵ���
			customDirection_ = sf::Vector2f(dir.x / len, dir.y / len);
			useCustomDirection_ = true;// ����Ϊʹ���Զ��巽��
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
	//��¼��һ��ÿ���ڵ��λ�ú��ƶ���������������ڵ����
	std::vector<sf::Vector2f> prevPositions(nodes_.size());
	std::vector<sf::Vector2f> prevMoveVecs(nodes_.size());
	for (size_t i = 0; i < nodes_.size(); ++i) {
		prevPositions[i] = nodes_[i].getPosition();
		prevMoveVecs[i] = nodes_[i].getMov();
	}

	//�����Ҫ���뵽��׼����
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
		//�ҵ���ӽ�moveVec_�ı�׼����
		for (int i = 0; i < 4; ++i) {
			float dot = moveVec_.x * directions[i].x + moveVec_.y * directions[i].y;
			if (dot > maxDot) {
				maxDot = dot;
				best = i;
			}
		}
		// ����direction_Ϊ����ı�׼����
		switch (best) {
		case 0: direction_ = Direction::Up; break;
		case 1: direction_ = Direction::Down; break;
		case 2: direction_ = Direction::Left; break;
		case 3: direction_ = Direction::Right; break;
		}
		// ������moveVec_Ҳ���뵽��׼����
		moveVec_ = directions[best];
		// ���ñ�־
		needtoreset_ = false;
	}

	// ���ʹ���Զ��巽��ֱ��ʹ��customDirection_�������ñ�־
	if(useCustomDirection_){
		moveVec_ = customDirection_;
		useCustomDirection_ = false;
		needtoreset_ = true;

	}

	else{
		// ���û���Զ��巽��ʹ�ü����߼�
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

	//����ͷ���λ�ú��ƶ�����
	nodes_[0].move(moveVec_.x * SnakeNode::Width * 1.5 , moveVec_.y * SnakeNode::Height * 1.5);
	nodes_[0].SetMov(moveVec_);

	//���������ڵ�λ�ú��ƶ�����
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
