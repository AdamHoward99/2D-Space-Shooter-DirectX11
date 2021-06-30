#include "Sprite.h"
#include "Backgrounds.h"
#include "assert.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Backgrounds::InitBgnd()
{
	//a sprite for each layer
	assert(mBgnd.empty());
	mBgnd.insert(mBgnd.begin(), BGND_LAYERS, Sprite(mD3D));

	assert(mBgndSG2.empty());
	mBgndSG2.insert(mBgndSG2.begin(), BGND_LAYERS, Sprite(mD3D));

	assert(mBgndSG3.empty());
	mBgndSG3.insert(mBgndSG3.begin(), BGND_LAYERS, Sprite(mD3D));


	pair<string, string> files[BGND_LAYERS]{
		{ "bgnd0","backgroundlayers/SpaceBackground.dds"},
		{"bgnd1","backgroundlayers/SpaceBackground.dds"}
	};

	//Backgrounds for Stage 2
	pair<string, string> filesSG2[BGND_LAYERS]{
		{"SG2bgnd0", "backgroundlayers/SpaceBackgroundSG2.dds"},
		{"SG2bgnd1", "backgroundlayers/SpaceBackgroundSG2.dds"}
	};

	//Backgrounds for Stage 3
	pair<string, string> filesSG3[BGND_LAYERS]{
		{"SG3bgnd0", "backgroundlayers/SpaceBackgroundSG3.dds"},
		{"SG3bgnd1", "backgroundlayers/SpaceBackgroundSG3.dds"}
	};

	int i = 0, j = 0, k = 0;
	//Stage 1
	for (auto& f : files)
	{
		//set each texture layer
		ID3D11ShaderResourceView *p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), f.second, f.first);
		if (!p)
			assert(false);
		mBgnd[i++].SetTex(*p);
	}

	//Stage 2
	for (auto& SGf : filesSG2)
	{
		ID3D11ShaderResourceView *p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), SGf.second, SGf.first);
		if (!p)
			assert(false);
		mBgndSG2[j++].SetTex(*p);
	}

	//Stage 3
	for (auto& SGf3 : filesSG3)
	{
		ID3D11ShaderResourceView *p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), SGf3.second, SGf3.first);
		if (!p)
			assert(false);
		mBgndSG3[k++].SetTex(*p);
	}
}

void Backgrounds::UpdateBgndStage1(float dTime)
{
	int i = 0;
	for (auto& s : mBgnd)
		s.Scroll(dTime*(i++)*SCROLL_SPEED, 0);
}

void Backgrounds::UpdateBgndStage2(float dTime)
{
	SCROLL_SPEED = 1000.f;
	int i = 0;
	for (auto& s : mBgndSG2)
		s.Scroll(dTime*(i++)*SCROLL_SPEED, 0);
}

void Backgrounds::UpdateBgndStage3(float dTime)
{
	SCROLL_SPEED = 4000.f;
	int i = 0;
	for (auto& s : mBgndSG3)
		s.Scroll(dTime*(i++)*SCROLL_SPEED, 0);
}

void Backgrounds::RenderBgndStage1(float dTime, DirectX::SpriteBatch & batch)
{
	for (auto& s : mBgnd)
		s.Draw(batch);
}

void Backgrounds::RenderBgndStage2(float dTime, DirectX::SpriteBatch & batch)
{
	for (auto& s : mBgndSG2)
		s.Draw(batch);
}

void Backgrounds::RenderBgndStage3(float dTime, DirectX::SpriteBatch & batch)
{
	for (auto& s : mBgndSG3)
		s.Draw(batch);
}