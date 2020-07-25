#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"
#include "PathNode.h"

class Plane final : public Sprite
{
public:
	Plane();
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void m_setPath(PathNode* a, PathNode* b, PathNode* c, PathNode* d);
	void m_move2TargetNode();

private:
	void m_buildAnimations();
	std::vector<PathNode*> m_pPatrolPath;
	PathNode* m_ptargetNode;
	int m_targetNodeIndex;

	float m_angle;
};

#endif /* defined (__PLANE__) */
