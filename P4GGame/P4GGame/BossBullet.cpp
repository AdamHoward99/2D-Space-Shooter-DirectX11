#include "BossBullet.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void BossBullet::Init(MyD3D& d3d)
{
	ID3D11ShaderResourceView* p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "bossMissile.dds");
	bbullet.SetTex(*p);
	bbullet.SetScale(Vector2(0.5f, 0.5f));
	bbullet.rotation = -1.5708f;
	active = false;
}

void BossBullet::Render(DirectX::SpriteBatch& batch)
{
	if (active)
		bbullet.Draw(batch);
}

void BossBullet::Update(float dTime)
{
	if (active)
	{
		bbullet.mPos.x -= BMISSILE_SPEED * dTime;
		if (bbullet.mPos.x < 0)
			active = false;
	}
}