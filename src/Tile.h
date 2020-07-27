#pragma once
#ifndef _TILE_H_
#define _TILE_H_
#include "DisplayObject.h"
#include "Frame.h"
#include "SpriteSheet.h"

class Tile : public DisplayObject
{
public:
	Tile();
	Tile(std::string text, bool obstacle);
	~Tile();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	Tile* clone() { return new Tile(texture, isImpassable); }
	bool getIsObstacle()
	{
		return isImpassable;
	}
private:
	SpriteSheet* spriteSheet;
	std::string texture;
	Frame* frame;
	bool isImpassable;
};
#endif

