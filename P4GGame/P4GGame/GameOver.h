#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class GameOver
{
public:
	GameOver(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:gameover(d3d), gameoverTitle(d3d), mpFont(mpFont), mD3D(d3d)
	{}

	void gameoverMode();
	void gameoverRender(DirectX::SpriteBatch& batch, int score);

private:
	Sprite gameover;
	Sprite gameoverTitle;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};