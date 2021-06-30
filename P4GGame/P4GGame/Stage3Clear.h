#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class Stage3Clear
{
public:
	Stage3Clear(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:mD3D(d3d), mpFont(mpFont), intro(d3d), stage3Title(d3d)
	{}

	void stage3Clear();
	void stage3ClearRender(DirectX::SpriteBatch& batch, float totalMissilesFired, float asteroidKillTotal, int score);

private:
	Sprite intro;
	Sprite stage3Title;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};