#include "Tile.h"
#include "TextureManager.h"

Tile::Tile()
{
	//TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");
	spriteSheet = TextureManager::Instance()->getSpriteSheet("tiles");

	
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	isImpassable = false;
	frame = &spriteSheet->getFrame("grass");

	setType(OBSTACLE);
}

Tile::Tile(std::string text, bool obstacle)
{
	//TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");
	spriteSheet = TextureManager::Instance()->getSpriteSheet("tiles");

	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
	getTransform()->position = glm::vec2(0, 0);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	texture = text;
	frame = &spriteSheet->getFrame(text);


	if (obstacle)
	{
		std::cout << spriteSheet->getFrame(text).name << "\n";
		setType(OBSTACLE);
		isImpassable = true;
	}
	else
	{
		setType(BACKGROUND);
		isImpassable = false;
	}
};

Tile::~Tile()
= default;

void Tile::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	
	// draw the target
	TextureManager::Instance()->drawFrame("tiles", &spriteSheet->getFrame(texture), x, y, getWidth(),getHeight(), 0, 255, true);
}

void Tile::update()
{
}

void Tile::clean()
{
}
