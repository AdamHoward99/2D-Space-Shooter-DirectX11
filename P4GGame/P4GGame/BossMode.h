#pragma once
#include <vector>
#include "Sprite.h"
#include "BossBullet.h"

class BossMode
{
public:
	BossMode(MyD3D& d3d)
		:d3d(d3d), boss(d3d), mBossMissile(d3d)
	{}

	float getBossHealth();
	void InitBoss();
	void UpdateBoss(float dTime);
	void damageBoss();
	void resetBossHealth();
	void updateMissile(float dTime);
	void InitBossBgnd();
	void UpdateBossBgnd(float dTime);
	void RenderBossBgnd(float dTime, DirectX::SpriteBatch & batch);

	Sprite boss;
	BossBullet mBossMissile;

private:
	std::vector<Sprite> mBgndboss; //Background for BOSS
	bool hitTop = false, hitBottom = true;
	float bossHealth = 1000.f;
	MyD3D& d3d;
	RECTF mPlayArea;	//don't go outside this
	float SPEED = 300.f;
	static const int BGND_LAYERS = 2;
	float SCROLL_SPEED = 50.f;
};