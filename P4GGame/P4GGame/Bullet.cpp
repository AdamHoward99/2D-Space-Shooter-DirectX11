#include "Bullet.h"
#include <iomanip>
#include <fstream>

#include "Game.h"
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const RECTF missileSpin[]{
	{ 0,  0, 53, 48},
	{ 54, 0, 107, 48 },
	{ 108, 0, 161, 48 },
	{ 162, 0, 220, 48 },
};

void Bullet::Init(MyD3D& d3d)
{
	vector<RECTF> frames2(missileSpin, missileSpin + sizeof(missileSpin) / sizeof(missileSpin[0]));
	ID3D11ShaderResourceView* p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "missile.dds", "missile", true, &frames2);

	bullet.SetTex(*p);
	bullet.GetAnim().Init(0, 3, 15, true);
	bullet.GetAnim().Play(true);
	bullet.SetScale(Vector2(0.5f, 0.5f));
	bullet.origin = Vector2((missileSpin[0].right - missileSpin[0].left) / 2.f, (missileSpin[0].bottom - missileSpin[0].top) / 2.f);
	active = false;
}

void Bullet::Render(SpriteBatch& batch)
{
	if (active)
		bullet.Draw(batch);
}

void Bullet::Update(float dTime)
{
	if (active)
	{
		bullet.mPos.x += MISSILE_SPEED * dTime;
		if (bullet.mPos.x > WinUtil::Get().GetClientWidth())
			active = false;
		bullet.GetAnim().Update(dTime);
	}
}