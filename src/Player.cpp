#include "Player.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "SoundManager.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/Pirate1.txt",
		"../Assets/sprites/Pirate1.png",
		"Pirate");


	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Pirate"));
	
	SoundManager::Instance().load("../Assets/audio/metalWalk.wav", "pWalk", SOUND_SFX);

	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	m_frameCount = 0;

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
	case PLAYER_DEAD:
		break;
	case PLAYER_HURT:
		break;
	case PLAYER_SHOOT:
		TextureManager::Instance()->playAnimation("Pirate", getAnimation("shoot"),
			x-10, y-10, 0.1f, 0, 255, true, (SDL_RendererFlip)m_FacingRight);
		break;
	case PLAYER_ATTACK:
		TextureManager::Instance()->playAnimation("Pirate", getAnimation("attack"),
			x, y, 0.1f, 0, 255, true, (SDL_RendererFlip)m_FacingRight);
		break;
	case PLAYER_IDLE:
		TextureManager::Instance()->playAnimation("Pirate", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, (SDL_RendererFlip)m_FacingRight);
		break;
	case PLAYER_RUN:
		TextureManager::Instance()->playAnimation("Pirate", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, (SDL_RendererFlip)m_FacingRight);
		break;
	default:
		break;
	}
	
}

void Player::update()
{
	switch (m_currentAnimationState)
	{
	case PLAYER_IDLE:
		Mix_HaltChannel(0); //moving sound channel
		m_walkingSoundPlaying = false;
		if (m_isMoving)
			setAnimationState(PLAYER_RUN);
		checkInput();
		break;
	case PLAYER_RUN:
		move();
		if (m_isMoving == false)
			setAnimationState(PLAYER_IDLE);
		checkInput();
		break;
	case PLAYER_SHOOT:
		returnToIdleState(60);
		break;
	case PLAYER_ATTACK:
		returnToIdleState(60);
		break;
	case PLAYER_HURT:
		returnToIdleState(15);
		break;
	case PLAYER_DEAD:
		break;

	default:
		break;
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
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("P-idle1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("P-idle2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("P-idle3"));
	
	setAnimation(idleAnimation);


	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("P-run1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("P-run2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("P-run3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("P-run4"));

	setAnimation(runAnimation);


	Animation shootAnimation = Animation();

	shootAnimation.name = "shoot";
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("P-gun1"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("P-gun2"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("P-gun3"));

	setAnimation(shootAnimation);

	Animation attackAnimation = Animation();

	attackAnimation.name = "attack";
	attackAnimation.frames.push_back(getSpriteSheet()->getFrame("P-sword1"));
	attackAnimation.frames.push_back(getSpriteSheet()->getFrame("P-sword2"));
	attackAnimation.frames.push_back(getSpriteSheet()->getFrame("P-sword3"));

	setAnimation(attackAnimation);

	Animation hurtAnimation = Animation();

	hurtAnimation.name = "hurt";
	hurtAnimation.frames.push_back(getSpriteSheet()->getFrame("P-hit"));
	hurtAnimation.frames.push_back(getSpriteSheet()->getFrame("P-hit"));

	setAnimation(hurtAnimation);

	Animation deadAnimation = Animation();

	deadAnimation.name = "dead";
	deadAnimation.frames.push_back(getSpriteSheet()->getFrame("P-dead"));

	setAnimation(deadAnimation);

}

void Player::checkInput()
{
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
			}

			if (EventManager::Instance().getGameController(0)->A_BUTTON)
			{
				shoot();
			}
			if (EventManager::Instance().getGameController(0)->B_BUTTON)
			{
				punch();
			}
		}
	}

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		m_isMoving = true;
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			getRigidBody()->velocity = glm::vec2(0.0f, -2.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			getRigidBody()->velocity = glm::vec2(0.0f, 2.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_FacingRight = false;

			getRigidBody()->velocity = glm::vec2(-2.0f, 0.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_FacingRight = true;

			getRigidBody()->velocity = glm::vec2(2.0f, 0.0f);
		}
		else
		{
			m_isMoving = false;
		}

		if (EventManager::Instance().getMouseButton(0))
		{
			shoot();
		}
		if (EventManager::Instance().getMouseButton(2))
		{
			punch();
		}
	}
}

void Player::returnToIdleState(int frames)
{
	m_frameCount++;
	if (m_frameCount > frames)
		setAnimationState(PLAYER_IDLE);
}

void Player::move()
{
	setAnimationState(PLAYER_RUN);
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= getRigidBody()->velocity * 0.9f;
	if (m_walkingSoundPlaying == false)
	{
		SoundManager::Instance().playSound("pWalk", -1, 0);
		m_walkingSoundPlaying = true;
	}
}

void Player::shoot()
{
	m_frameCount = 0;
	setAnimationState(PLAYER_SHOOT);
}

void Player::punch()
{
	m_frameCount = 0;
	setAnimationState(PLAYER_ATTACK);
}
