#include "EnemyBase.h"
#include "EnemyBase.h"
#include "EnemyBase.h"
#include "EnemyBase.h"
#include "TextureManager.h"
#include "Util.h"
#include "DebugKeys.h"
#include "PlayerAnimationState.h"

EnemyBase::EnemyBase()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/Assassin.txt",
		"../Assets/sprites/Assassin.png",
		"Assassin");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Assassin"));

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);
	
	m_bLookingRight = true;
	m_health = 5;
	m_damage = 1;

	getTransform()->position = glm::vec2(400.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 3;
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_buildAnimations();

	m_targetNodeIndex = 1;
}

EnemyBase::~EnemyBase()
= default;

void EnemyBase::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	switch (m_animationState)
	{
	case ENEMY_IDLE:
		TextureManager::Instance()->playAnimation(
			"Assassin", getAnimation("A_idle"),
			x, y, 0.1f, 0, 255, true, (SDL_RendererFlip)m_bLookingRight);
		break;
	case ENEMY_RUN:
		TextureManager::Instance()->playAnimation(
			"Assassin", getAnimation("A_run"),
			x, y, 0.15f, 0, 255, true, (SDL_RendererFlip)m_bLookingRight);
		break;
	case ENEMY_ATTACK:
		break;
	case ENEMY_HURT:
		TextureManager::Instance()->playAnimation(
			"Assassin", getAnimation("A_hurt"),
			x, y, 0.1f, 30, 255, true, (SDL_RendererFlip)m_bLookingRight);
		break;
	case ENEMY_DEAD:
		TextureManager::Instance()->playAnimation(
			"Assassin", getAnimation("A_dead"),
			x, y, 0.1f, -90, 255, true);
		break;
	
	default:
		break;
	}
}

void EnemyBase::update()
{
	switch (m_animationState)
	{
	case ENEMY_IDLE:
		if (m_bPatrolMode)
			setAnimationState(ENEMY_RUN);
		break;
	case ENEMY_RUN:
		m_move2TargetNode();
		if (!m_bPatrolMode)
			setAnimationState(ENEMY_IDLE);
			
		break;
	case ENEMY_ATTACK:
		m_frameCounter++;
		if(m_frameCounter > 60)
			setAnimationState(ENEMY_IDLE);
		break;
	case ENEMY_HURT:
		m_frameCounter++;
		if (m_frameCounter > 15)
			setAnimationState(ENEMY_IDLE);
		break;
	case ENEMY_DEAD:

		break;
	default:

		break;
	}

	if (m_animationState != ENEMY_HURT && m_health <= 0)
		setAnimationState(ENEMY_DEAD);

}

void EnemyBase::clean()
{
}

void EnemyBase::m_setPath(PathNode * a, PathNode * b, PathNode * c, PathNode * d)
{
	m_pPatrolPath.push_back(a);
	m_pPatrolPath.push_back(b);
	m_pPatrolPath.push_back(c);
	m_pPatrolPath.push_back(d);

}

void EnemyBase::m_move2TargetNode()
{
	setAnimationState(ENEMY_RUN);
	m_ptargetNode = m_pPatrolPath[m_targetNodeIndex];
	auto targetVector = Util::normalize(m_ptargetNode->getTransform()->position - getTransform()->position);
	
	if (targetVector.x > 0.5f)
		m_bLookingRight = true;
	else if(targetVector.x <= -0.5f)
		m_bLookingRight = false;

	std::cout << targetVector.x << std::endl;

	getRigidBody()->velocity = targetVector;
	getTransform()->position += getRigidBody()->velocity * getRigidBody()->maxSpeed;
	if (Util::distance(getTransform()->position, m_ptargetNode->getTransform()->position) < 5.0f)
	{
		m_targetNodeIndex++;
		if (m_targetNodeIndex > m_pPatrolPath.size() - 1)
			m_targetNodeIndex = 0;
	}
}

void EnemyBase::setPatrolMode(bool patrol)
{
	m_bPatrolMode = patrol;
	if (patrol)
		setAnimationState(ENEMY_RUN);
	else
		setAnimationState(ENEMY_IDLE);
}

void EnemyBase::takeDamage(int dmg)
{
	m_frameCounter = 0;
	setAnimationState(ENEMY_HURT);	
	m_health -= dmg;
}

void EnemyBase::setAnimationState(EnemyAminationState nState)
{
	m_frameCounter = 0;
	m_animationState = nState;
}

void EnemyBase::m_buildAnimations()
{
	Animation A_idleAnimation = Animation();

	A_idleAnimation.name = "A_idle";
	A_idleAnimation.frames.push_back(getSpriteSheet()->getFrame("A-idle1"));
	A_idleAnimation.frames.push_back(getSpriteSheet()->getFrame("A-idle2"));
	A_idleAnimation.frames.push_back(getSpriteSheet()->getFrame("A-idle3"));

	setAnimation(A_idleAnimation);

	Animation A_runAnimation = Animation();

	A_runAnimation.name = "A_run";
	A_runAnimation.frames.push_back(getSpriteSheet()->getFrame("A-run1"));
	A_runAnimation.frames.push_back(getSpriteSheet()->getFrame("A-run2"));
	A_runAnimation.frames.push_back(getSpriteSheet()->getFrame("A-run3"));

	setAnimation(A_runAnimation);

	Animation A_hurtAnimation = Animation();

	A_hurtAnimation.name = "A_hurt";
	A_hurtAnimation.frames.push_back(getSpriteSheet()->getFrame("A-hit"));
	A_hurtAnimation.frames.push_back(getSpriteSheet()->getFrame("A-hit"));

	setAnimation(A_hurtAnimation);

	Animation A_deadAnimation = Animation();

	A_deadAnimation.name = "A_dead";
	A_deadAnimation.frames.push_back(getSpriteSheet()->getFrame("A-dead"));

	setAnimation(A_deadAnimation);
}