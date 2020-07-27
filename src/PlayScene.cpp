#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_bDebugMode)
	{
		auto LOSColour = (!m_bPlayerHasLOS) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		Util::DrawLine(m_pPlayer->getTransform()->position, m_pEnemySprite->getTransform()->position, LOSColour);

		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() *0.5f),
			m_pPlayer->getWidth(), m_pPlayer->getHeight());

		Util::DrawRect(m_pEnemySprite->getTransform()->position - glm::vec2(m_pEnemySprite->getWidth() * 0.5f, m_pEnemySprite->getHeight() * 0.5f),
			m_pEnemySprite->getWidth(), m_pEnemySprite->getHeight());

		Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5f, m_pObstacle->getHeight() * 0.5f),
			m_pObstacle->getWidth(), m_pObstacle->getHeight());

		m_displayGrid();

		//m_displayGridLOS();
	}
}

void PlayScene::update()
{
	updateDisplayList();

	m_bPlayerHasLOS = CollisionManager::LOSCheck(m_pPlayer, m_pEnemySprite, m_pObstacle);

	CollisionManager::AABBCheck(m_pPlayer, m_pEnemySprite);

	CollisionManager::AABBCheck(m_pPlayer, m_pObstacle);

	m_setGridLOS();

	m_pEnemySprite->update();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	m_pPlayer->update();

	// H KEY Section
	
	if(!m_bDebugKeys[H_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			// toggles Debug Mode
			m_bDebugMode = !m_bDebugMode;

			m_bDebugKeys[H_KEY] = true;

			if (m_bDebugMode)
			{
				std::cout << "DEBUG Mode On" << std::endl;
			}
			else
			{
				std::cout << "DEBUG Mode Off" << std::endl;
			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_bDebugKeys[H_KEY] = false;
	}

	// K KEY Section
	
	if (!m_bDebugKeys[K_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			std::cout << "DEBUG: Enemies taking damage!" << std::endl;
			m_pEnemySprite->takeDamage(1);
			m_bDebugKeys[K_KEY] = true;
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_bDebugKeys[K_KEY] = false;
	}

	// P KEY Section

	if (!m_bDebugKeys[P_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			// toggles Patrol Mode
			m_bPatrolMode = !m_bPatrolMode;

			m_bDebugKeys[P_KEY] = true;

			if (m_bPatrolMode)
			{
				std::cout << "DEBUG: Patrol Mode On" << std::endl;
			}
			else
			{
				std::cout << "DEBUG: Patrol Mode Off" << std::endl;
			}
			m_pEnemySprite->setPatrolMode(m_bPatrolMode);
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_bDebugKeys[P_KEY] = false;
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::m_buildGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto pathNode = new PathNode();
			pathNode->getTransform()->position = glm::vec2(pathNode->getWidth() * col + Config::TILE_SIZE * 0.5, pathNode->getHeight() * row + Config::TILE_SIZE * 0.5);
			m_pGrid.push_back(pathNode);
		}
	}

	std::cout << "Number of Nodes: " << m_pGrid.size() << std::endl;
}

void PlayScene::m_displayGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto colour = (!m_pGrid[row*Config::COL_NUM + col]->getLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position - glm::vec2(m_pGrid[row * Config::COL_NUM + col]->getWidth() * 0.5f, m_pGrid[row * Config::COL_NUM + col]->getHeight() * 0.5f),
				Config::TILE_SIZE, Config::TILE_SIZE);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position, 5, 5, colour);
		}
	}
}

void PlayScene::m_displayGridLOS()
{
	for (auto node : m_pGrid)
	{
		if(!node->getLOS())
		{
			auto colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

			Util::DrawLine(node->getTransform()->position, m_pPlayer->getTransform()->position, colour);
		}
		
	}
}

void PlayScene::m_setGridLOS()
{
	for (auto node : m_pGrid)
	{
		node->setLOS(CollisionManager::LOSCheck(node, m_pPlayer, m_pObstacle));
	}
}

void PlayScene::start()
{
	m_bPlayerHasLOS = false;

	m_buildGrid();
	
	m_bDebugMode = false;
	m_bPatrolMode = false;
	
	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position.y = 400;
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Plane Sprite
	m_pEnemySprite = new EnemyBase();
	m_pEnemySprite->getTransform()->position.y = 40;
	m_pEnemySprite->getTransform()->position.x = 40;
	addChild(m_pEnemySprite);

	// Obstacle Texture
	m_pObstacle = new Obstacle();
	addChild(m_pObstacle);
	
	m_pEnemySprite->m_setPath(m_pGrid[0], m_pGrid[Config::COL_NUM - 1],
		m_pGrid[Config::COL_NUM * Config::ROW_NUM - 1], m_pGrid[Config::COL_NUM * Config::ROW_NUM - Config::COL_NUM]);

	SoundManager::Instance().load("../Assets/audio/game.mp3", "bgm", SOUND_MUSIC);
	SoundManager::Instance().playMusic("bgm");
	SoundManager::Instance().setMusicVolume(16);
	SoundManager::Instance().setSoundVolume(100);
	
}


