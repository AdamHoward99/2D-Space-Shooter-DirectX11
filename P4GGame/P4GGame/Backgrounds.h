#pragma once
#include <vector>
#include "Sprite.h"

class Backgrounds
{
public:
	Backgrounds(MyD3D& d3d)
		:mD3D(d3d)
	{}

	void InitBgnd();
	void UpdateBgndStage1(float dTime);
	void UpdateBgndStage2(float dTime);
	void UpdateBgndStage3(float dTime);

	void RenderBgndStage1(float dTime, DirectX::SpriteBatch & batch);
	void RenderBgndStage2(float dTime, DirectX::SpriteBatch & batch);
	void RenderBgndStage3(float dTime, DirectX::SpriteBatch & batch);
	float SCROLL_SPEED = 250.f;

private:
	std::vector<Sprite> mBgnd; //parallax layers
	std::vector<Sprite> mBgndSG2; //Background for Stage 2
	std::vector<Sprite> mBgndSG3; //Background for Stage 3
	static const int BGND_LAYERS = 2;
	MyD3D& mD3D;
};