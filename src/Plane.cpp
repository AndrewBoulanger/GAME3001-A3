#include "Plane.h"
#include "TextureManager.h"
#include "Util.h"

Plane::Plane()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getTransform()->position = glm::vec2(400.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 5;
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_buildAnimations();

	m_targetNodeIndex = 1;
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->playAnimation(
		"spritesheet", getAnimation("plane"),
		x, y, 0.5f, m_angle, 255, true);
}

void Plane::update()
{
}

void Plane::clean()
{
}

void Plane::m_setPath(PathNode* a, PathNode* b, PathNode* c, PathNode* d)
{
	m_pPatrolPath.push_back(a);  
	m_pPatrolPath.push_back(b);  
	m_pPatrolPath.push_back(c);  
	m_pPatrolPath.push_back(d);  

}

void Plane::m_move2TargetNode()
{
	m_ptargetNode = m_pPatrolPath[m_targetNodeIndex];
	auto targetVector = Util::normalize(m_ptargetNode->getTransform()->position - getTransform()->position);
	
	m_angle = (targetVector.y > 0) ? 180: targetVector.x * 90;  //sets the 4 possible angles of the plane
	
	getRigidBody()->velocity = targetVector;
	getTransform()->position += getRigidBody()->velocity * getRigidBody()->maxSpeed;
	if (getTransform()->position == m_ptargetNode->getTransform()->position)
	{
		m_targetNodeIndex++;
		if (m_targetNodeIndex > m_pPatrolPath.size() - 1)
			m_targetNodeIndex = 0;
	}
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "plane";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane1"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane2"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane3"));

	setAnimation(planeAnimation);
}