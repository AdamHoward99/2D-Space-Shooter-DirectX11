#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class Stage1Clear
{
public:
	Stage1Clear(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:mD3D(d3d), mpFont(mpFont), intro(d3d), stage1Title(d3d)
	{}


	void stage1Clear();
	void stage1ClearRender(DirectX::SpriteBatch& batch, float totalMissilesFired, float asteroidKillTotal, int score);

private:
	Sprite intro;
	Sprite stage1Title;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};