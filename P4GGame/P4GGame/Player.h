#pragma once
#include "Sprite.h"
#include "Bullet.h"

const RECTF thrustAnim[]{
	{ 0,  0, 15, 16},
	{ 16, 0, 31, 16 },
	{ 32, 0, 47, 16 },
	{ 48, 0, 64, 16 },
};

class Player
{
public:
	Player(MyD3D& d3d)
		:mPlayer(d3d), mThrust(d3d), d3d(d3d)
	{}

	void InitPlayer();
	void UpdateInput(float dTime);
	void UpdateThrust(float dTime);
	bool playerDied();

	Sprite mPlayer;		//jet
	Sprite mThrust;		//flames out the back
	float mThrusting = 0;
	RECTF mPlayArea;	//don't go outside this	
	const float SPEED = 250.f;
	bool hasDied = false;
private:
	MyD3D& d3d;
	const float MOUSE_SPEED = 5000;
	const float PAD_SPEED = 500;
};