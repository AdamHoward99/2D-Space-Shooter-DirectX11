#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class BossClear
{
public:
	BossClear(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:mD3D(d3d), mpFont(mpFont), intro(d3d), EndTitle(d3d)
	{}

	void bossClear();
	void bossClearRender(DirectX::SpriteBatch& batch, int score);

private:
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
	Sprite EndTitle;
	Sprite intro;
};