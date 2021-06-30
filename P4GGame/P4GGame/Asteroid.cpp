#include "Asteroid.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Asteroid::Init()
{
	vector<RECTF> frames2;
	frames2.insert(frames2.begin(), 8 * 8, RECTF());
	const float as_sz = 102;
	int c(0);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			frames2[c++] = RECTF{ j * as_sz, i * as_sz, j * as_sz + as_sz, i * as_sz + as_sz };
	ID3D11ShaderResourceView* p = spr.GetD3D().GetCache().LoadTexture(&spr.GetD3D().GetDevice(), "asteroid.dds", "asteroid", true, &frames2);

	spr.SetTex(*p);
	spr.GetAnim().Init(0, 31, 15, true);
	spr.GetAnim().Play(true);
	spr.SetScale(Vector2(0.5f, 0.5f));
	spr.origin = Vector2(as_sz / 2.f, as_sz / 2.f);
	active = false;
}

void Asteroid::Render(SpriteBatch& batch)
{
	if (active)
		spr.Draw(batch);
}

void Asteroid::Update(float dTime)
{
	if (active)
	{
		float radius = spr.GetScreenSize().Length() / 2.f;
		spr.mPos.x -= asteroid_speed * dTime;
		if (spr.mPos.x < -radius)
			active = false;
		spr.GetAnim().Update(dTime);
	}
}