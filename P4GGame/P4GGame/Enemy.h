#pragma once
#include "Sprite.h"

struct Enemy
{
	Enemy(MyD3D& d3d)
		:enemySpr(d3d)
	{}
	Sprite enemySpr;
	bool active = false;
	void Init(MyD3D& d3d);
	void Render(DirectX::SpriteBatch& batch);
	void Update(float dTime);
	float MOVING_SPEED = 200;
	float enemySpawnRate = 6.f;
	float enemyLastSpawn = 0;
};