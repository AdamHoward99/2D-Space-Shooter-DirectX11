#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class Stage2Clear
{
public:
	Stage2Clear(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:mD3D(d3d), mpFont(mpFont), intro(d3d), stage2Title(d3d)
	{}

	void stage2Clear();
	void stage2ClearRender(DirectX::SpriteBatch& batch, float totalMissilesFired, float asteroidKillTotal, int score);

private:
	Sprite intro;
	Sprite stage2Title;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};