#pragma once
#include "Sprite.h"
#include "SpriteFont.h"

class ControlMenu
{
public:
	ControlMenu(MyD3D& d3d, DirectX::SpriteFont *mpFont)
		:intro(d3d), mD3D(d3d), mpFont(mpFont)
	{}

	void controlMode();
	void controlRender(DirectX::SpriteBatch& batch);
	void Release();

private:
	DirectX::SpriteFont *mpFont;
	MyD3D& mD3D;
	Sprite intro;
};