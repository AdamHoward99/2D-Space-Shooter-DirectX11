#include "Enemy.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Enemy::Init(MyD3D& d3d)
{
	ID3D11ShaderResourceView* p = enemySpr.GetD3D().GetCache().LoadTexture(&enemySpr.GetD3D().GetDevice(), "enemyShip.dds");
	enemySpr.SetTex(*p);
	enemySpr.SetScale(Vector2(0.15f, 0.15f));
	enemySpr.origin = Vector2(enemySpr.mPos.x / 2.f, 270);
	active = false;
}

void Enemy::Update(float dTime)
{
	if (active)
	{
		float radius = enemySpr.GetScreenSize().Length() / 2.f;
		enemySpr.mPos.x -= MOVING_SPEED * dTime;
		if (enemySpr.mPos.x < -radius)
			active = false;
	}
}

void Enemy::Render(DirectX::SpriteBatch& batch)
{
	if (active)
		enemySpr.Draw(batch);
}