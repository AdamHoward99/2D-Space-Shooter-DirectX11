#pragma once
#include "Sprite.h"

struct Bullet
{
	Bullet(MyD3D& d3d)
		:bullet(d3d)
	{}
	Sprite bullet;
	bool active = false;

	void Init(MyD3D& d3d);
	void Render(DirectX::SpriteBatch& batch);
	void Update(float dTime);
	const float MISSILE_SPEED = 300;
};