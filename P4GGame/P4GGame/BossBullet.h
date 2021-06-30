#pragma once

#include "Sprite.h"

struct BossBullet
{
	BossBullet(MyD3D& d3d)
		:bbullet(d3d)
	{}
	Sprite bbullet;
	bool active = false;

	void Init(MyD3D& d3d);
	void Render(DirectX::SpriteBatch& batch);
	void Update(float dTime);
	const float BMISSILE_SPEED = 600;
};