#pragma once
#ifndef __ENEMYBASE__
#define __ENEMYBASE__
#include "Sprite.h"
#include "PathNode.h"
#include "PlayerAnimationState.h"
class EnemyBase :
    public Sprite
{
public:
	EnemyBase();
	~EnemyBase();
	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void m_setPath(PathNode* a, PathNode* b, PathNode* c, PathNode* d);
	void m_move2TargetNode();
	void setPatrolMode(bool patrol);
	void takeDamage(int dmg);
	void setAnimationState(EnemyAminationState nState);

private:
	int m_health, m_damage, m_frameCounter;
	void m_buildAnimations();
	std::vector<PathNode*> m_pPatrolPath;
	PathNode* m_ptargetNode;
	int m_targetNodeIndex;
	bool m_bLookingRight, m_bPatrolMode;
	EnemyAminationState m_animationState;

	float m_angle;
};

#endif /* defined (__ENEMYBASE__) */
