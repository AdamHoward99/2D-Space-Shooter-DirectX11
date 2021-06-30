#pragma once
#include "SpriteFont.h"
#include "Sprite.h"

class Score
{
public:
	Score(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:mCoinSpin(d3d), mpFont(mpFont), mD3D(d3d)
	{}

	int getScore();
	void addToScore();
	void resetScore();
	void InitCoinAnim();

	Sprite mCoinSpin;
private:
	int score = 0;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};