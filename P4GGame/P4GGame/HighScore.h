#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class HighScoreMenu
{
public:
	HighScoreMenu(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:intro(d3d), mD3D(d3d), mpFont(mpFont)
	{}

	void highScoreMode();
	void highScoreRender(DirectX::SpriteBatch& batch);
	void Release();

private:
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
	Sprite intro;
};