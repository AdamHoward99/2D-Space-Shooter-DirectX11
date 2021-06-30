#pragma once

#include "Sprite.h"

namespace GC
{
	const float asteroid_spawn_rate = 0.7f;		//Time between spawning next Asteroid
	const float asteroid_spawn_inc = 1;
	const float asteroid_max_spawn_delay = 4;
	const int roid_cache = 40;		//Amount of Asteroids that can be on screen at same time
}

struct Asteroid
{
	Asteroid(MyD3D& d3d)
		:spr(d3d)
	{}
	Sprite spr;
	bool active = false;	//should it render and animate?

	float asteroid_speed = 100;

	//setup
	void Init();
	void Render(DirectX::SpriteBatch& batch);
	void Update(float dTime);
};