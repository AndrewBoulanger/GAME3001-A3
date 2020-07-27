#pragma once
#ifndef _TILE_H_
#define _TILE_H_
#include "DisplayObject.h"

class Tile : public DisplayObject
{
public:
	Tile();
	Tile(float x, float  y, float width, float  height, bool obstacle);
	~Tile();

	// DisplayObject Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:

};
#endif

