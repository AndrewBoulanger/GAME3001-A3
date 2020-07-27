#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "EnemyBase.h"
#include "Player.h"
#include "Button.h"
#include "DebugKeys.h"
#include "Obstacle.h"
#include "PathNode.h"
#include "Tile.h"
#include <map>

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// PRIVATE FUNCTIONS
	void m_buildGrid();

	void m_displayGrid();

	void m_displayGridLOS();

	void m_setGridLOS();

	void m_loadLevel();

	// MEMBER VARIABLES
	glm::vec2 m_mousePosition;

	EnemyBase* m_pEnemySprite;
	Player* m_pPlayer;
	bool m_playerFacingRight;
	bool m_bPlayerHasLOS;

	Obstacle* m_pObstacle;

	bool m_bDebugMode;
	bool m_bPatrolMode;

	bool m_bDebugKeys[NUM_OF_DEBUG_KEYS];

	std::vector<PathNode*> m_pGrid;
	std::vector<Tile*> m_pTiles;
	std::map<char, Tile*> m_tileSet;

	std::vector<PathNode*> m_pPatrolPath;
	int m_targetNodeIndex;
	PathNode* m_ptargetNode;
	
};

#endif /* defined (__PLAY_SCENE__) */