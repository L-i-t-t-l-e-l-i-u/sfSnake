#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>

#include "SnakeNode.h"
#include "Fruit.h"
#include "Wall.h"

namespace sfSnake
{
	enum class Direction
	{
		Left, Right, Up, Down
	};

class Snake
{
public:
	Snake();

	void handleInput(sf::RenderWindow& window);
	void update(sf::Time delta);
	void render(sf::RenderWindow& window);

	void checkFruitCollisions(std::vector<Fruit>& fruits);
	void checkWallCollisions(std::vector<Wall> walls);

	bool endgame() const;
	void die();

	unsigned getSize() const;


    const std::vector<SnakeNode>& getNodes() const {
        return nodes_;
    }

private:
	void move();
	void grow();
	void checkEdgeCollisions();
	void checkSelfCollisions();
	void initNodes();
	sf::Vector2f customDirection_{ 0, 0 }; // 鼠标控制的方向向量
	bool useCustomDirection_ = false;    // 是否使用鼠标方向
	bool needtoreset_ = false;

	bool die_;


	sf::Vector2f position_;
	Direction direction_;

	sf::SoundBuffer pickupBuffer_;
	sf::Sound pickupSound_;

	sf::SoundBuffer dieBuffer_;
	sf::Sound dieSound_;



	std::vector<SnakeNode> nodes_;
	sf::Vector2f moveVec_;

	static const int InitialSize;
};
}

#endif