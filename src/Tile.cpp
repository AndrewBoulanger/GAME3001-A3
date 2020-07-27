#include "Tile.h"

#include "TextureManager.h"

Tile::Tile()
{
	//TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	const glm::vec2 size;
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(OBSTACLE);
}

Tile::Tile(float  x, float y, float width, float height, bool obstacle)
{
	//TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	const glm::vec2 size;
	setWidth(width);
	setHeight(height);
	getTransform()->position = glm::vec2(x, y);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	if (obstacle)
	{
		setType(OBSTACLE);
	}
	else
	{
		setType(BACKGROUND);
	}
}

Tile::~Tile()
= default;

void Tile::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("obstacle", x, y, 0, 255, true);
}

void Tile::update()
{
}

void Tile::clean()
{
}
