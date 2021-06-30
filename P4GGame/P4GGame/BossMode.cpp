#include "BossMode.h"
#include "WindowUtils.h"
#include <vector>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void BossMode::InitBoss()
{
	ID3D11ShaderResourceView *p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "boss.dds");
	boss.SetTex(*p);
	boss.SetScale(Vector2(0.4f, 0.4f));
	boss.origin = boss.GetTexData().dim / 2.f;
	boss.rotation = -PI / 2.f;

	//setup the play area
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mPlayArea.left = boss.GetScreenSize().x*0.6f;
	mPlayArea.top = boss.GetScreenSize().y * 0.3f;
	mPlayArea.right = w - mPlayArea.left;
	mPlayArea.bottom = h * 0.85f;

	boss.mPos = Vector2(700, (mPlayArea.bottom - mPlayArea.top) / 2.f);
}

void BossMode::UpdateBoss(float dTime)
{
	//Boss Movement
	if ((hitBottom) && (!hitTop))		//If the enemy has hit the bottom of the screen, move up to the top
	{
		if (boss.mPos.y > mPlayArea.top)
			boss.mPos.y -= SPEED * dTime;
		else if (boss.mPos.y < mPlayArea.top)
			boss.mPos.y = mPlayArea.top;
	}

	if ((!hitBottom) && (hitTop))
	{
		if (boss.mPos.y < mPlayArea.bottom)
			boss.mPos.y += SPEED * dTime;
		else if (boss.mPos.y > mPlayArea.bottom)
			boss.mPos.y = mPlayArea.bottom;
	}

	if (boss.mPos.y == mPlayArea.bottom)
	{
		hitBottom = true;
		hitTop = false;
	}

	if (boss.mPos.y == mPlayArea.top)
	{
		hitBottom = false;
		hitTop = true;
	}

	//Collision between player and boss sprites
	//if ((boss.mPos - mPlayer.mPlayer.mPos).Length() < 150)
	//{
	//	hasDied = true;
	//	audio.audio.GetSfxMgr()->Play("largeExplosion", false, false, &audio.sfxHandle, 0.4f);
	//	return;
	//}
}

float BossMode::getBossHealth()
{
	return bossHealth;
}

void BossMode::damageBoss()
{
	bossHealth -= 50.f;
}

void BossMode::resetBossHealth()
{
	bossHealth = 1000.f;
}

void BossMode::updateMissile(float dTime)
{
	mBossMissile.active = true;
	if (mBossMissile.bbullet.mPos.x < 0)
		mBossMissile.bbullet.mPos = Vector2(550, boss.mPos.y);
	mBossMissile.Update(dTime);
}

void BossMode::InitBossBgnd()
{
	assert(mBgndboss.empty());
	mBgndboss.insert(mBgndboss.begin(), BGND_LAYERS, Sprite(d3d));


	pair<string, string> filesBoss[BGND_LAYERS]{
		{ "Bossbgnd0","backgroundlayers/BossBackground.dds"},
		{"Bossbgnd1","backgroundlayers/BossBackground.dds"}
	};

	int i = 0;
	for (auto& f : filesBoss)
	{
		//set each texture layer
		ID3D11ShaderResourceView *p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), f.second, f.first);
		if (!p)
			assert(false);
		mBgndboss[i++].SetTex(*p);
	}
}

void BossMode::UpdateBossBgnd(float dTime)
{
	int i = 0;
	for (auto& s : mBgndboss)
		s.Scroll(dTime*(i++)*SCROLL_SPEED, 0);
}

void BossMode::RenderBossBgnd(float dTime, DirectX::SpriteBatch & batch)
{
	for (auto& s : mBgndboss)
		s.Draw(batch);
}