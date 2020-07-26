#include "Player.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "SoundManager.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");
	SoundManager::Instance().load("audio/metalWalk.wav", "pWalk", SOUND_SFX);

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));
	
	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	getTransform()->position = glm::vec2(600.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 10.0f;
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
	
}

void Player::update()
{

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			m_isMoving = true;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_Y > deadZone)
			{
				getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_Y < -deadZone)
			{
				getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_FacingRight = true;

				getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_FacingRight = false;

				getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			}
			else
			{
				m_isMoving = false;
				if (m_FacingRight)
				{
					setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		m_isMoving = true;
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_FacingRight = false;

			getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_FacingRight = true;

			getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
		}
		else
		{
			m_isMoving = false;
			if (m_FacingRight)
			{
				setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}

	if (m_isMoving)
	{
		m_FacingRight ? setAnimationState(PLAYER_RUN_RIGHT) : setAnimationState(PLAYER_RUN_LEFT);
		getTransform()->position += getRigidBody()->velocity;
		getRigidBody()->velocity *= getRigidBody()->velocity * 0.9f;
		if (m_walkingSoundPlaying == false)
		{
			SoundManager::Instance().playSound("pWalk", 0, 0);
			m_walkingSoundPlaying = true;
		}
	}
	else
	{
		m_walkingSoundPlaying = false;
	}
	
}

void Player::clean()
{
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("megaman-run-3"));

	setAnimation(runAnimation);
}
