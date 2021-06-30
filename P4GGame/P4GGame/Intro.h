#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class Intro
{
public:
	Intro(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:intro(d3d), title(d3d), mD3D(d3d), mpFont(mpFont)
	{}
	void introMode();
	void introRender(DirectX::SpriteBatch& batch);
	void Release();

private:
	Sprite intro;
	Sprite title;
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
};