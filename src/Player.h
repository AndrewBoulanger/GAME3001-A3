#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;



	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	bool m_isMoving, m_walkingSoundPlaying;
	bool m_FacingRight;
	void m_buildAnimations();

	void shoot();
	void punch();

	PlayerAnimationState m_currentAnimationState;
	SDL_Texture* extraSprites;
};

#endif /* defined (__PLAYER__) */