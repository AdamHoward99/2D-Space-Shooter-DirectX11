#include "Score.h"
#include "WindowUtils.h"
#include <vector>
#include "Sprite.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const RECTF coinSpin[]
{
	{0, 0, 15, 15},
	{16, 0, 31, 15},
	{32, 0, 47, 15},
	{48, 0, 63, 15},
	{0, 16, 15, 31},
	{16, 16, 31, 31},
	{32, 16, 47, 31},
	{48, 16, 63, 31}
};

int Score::getScore()
{
	return score;
}

void Score::addToScore()
{
	score += 40;
}

void Score::resetScore()
{
	score = 0;
}

void Score::InitCoinAnim()
{
	vector<RECTF> frames(coinSpin, coinSpin + sizeof(coinSpin) / sizeof(coinSpin[0]));
	ID3D11ShaderResourceView *p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "coin.dds", "coin", true, &frames);
	mCoinSpin.SetTex(*p);
	mCoinSpin.SetScale(Vector2(2, 2));
	mCoinSpin.GetAnim().Init(0, 7, 15, true);
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mCoinSpin.mPos = Vector2(w*0.8f, h*0.9f);
	mCoinSpin.GetAnim().Play(true);
}