#include "PlayMode.h"
#include "Game.h"
#include "WindowUtils.h"

#include <iomanip>
#include <fstream>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayMode::PlayMode(MyD3D & d3d)
	:mD3D(d3d), mPlayer(d3d), mMissile(d3d), mScore(d3d, mpFont),
	enemy(d3d), mBgnd(d3d), mBoss(d3d)
{
	mScore.InitCoinAnim();
	mBgnd.InitBgnd();
	mpFont = new SpriteFont(&d3d.GetDevice(), L"data/fonts/comicSansMS.spritefont");
	assert(mpFont);
	initAsteroids();
	InitEnemy();
	mPlayer.InitPlayer();
	mMissile.Init(d3d);
	mBoss.InitBoss();
	mBoss.InitBossBgnd();
	mBoss.mBossMissile.Init(d3d);
	audio.audio.Initialise();
	audio.audio.GetSongMgr()->Play("menuTheme", true, false, &audio.sfxHandle, 0.4f);
}

void PlayMode::UpdateMissile(float dTime)
{
	if (!mMissile.active && Game::sMKIn.IsPressed(VK_SPACE) && !(stage == Stages::STAGE3))
	{
		mMissile.active = true;
		mMissile.bullet.mPos = Vector2(mPlayer.mPlayer.mPos.x + mPlayer.mPlayer.GetScreenSize().x / 2.f, mPlayer.mPlayer.mPos.y);
		totalMissilesFired++;
		audio.audio.GetSfxMgr()->Play("MAIMER", false, false, &audio.sfxHandle, 0.1f);
	}
	mMissile.Update(dTime);

	if (stage == Stages::BOSS)
	{
		
		//Collision between the players missile and the boss
		if (mMissile.active && (mBoss.boss.mPos - mMissile.bullet.mPos).Length() < 150)
		{
			mMissile.active = false;
			audio.audio.GetSfxMgr()->Play("smallExplosion", false, false, &audio.sfxHandle, 0.3f);
			mBoss.damageBoss();		//Takes 50 from boss' health
		}

		//Collision between the player and the boss' missile
		if (mBoss.mBossMissile.active && (mBoss.mBossMissile.bbullet.mPos - mPlayer.mPlayer.mPos).Length() < 35)
		{
			mPlayer.hasDied = true;
			mBoss.mBossMissile.active = false;
			mBoss.mBossMissile.bbullet.mPos = Vector2(100.f, -100.f);
			audio.audio.GetSfxMgr()->Play("largeExplosion", false, false, &audio.sfxHandle, 0.2f);
			return;
		}

			//Collision between player and boss sprites
		if ((mBoss.boss.mPos - mPlayer.mPlayer.mPos).Length() < 150)
		{
			mPlayer.hasDied = true;
			audio.audio.GetSfxMgr()->Play("largeExplosion", false, false, &audio.sfxHandle, 0.2f);
			return;
		}
	}
}

void PlayMode::Update(float dTime)
{
	audio.audio.Update();
	
	if(stage == Stages::STAGE1)		mBgnd.UpdateBgndStage1(dTime);
	else if (stage == Stages::STAGE2)		mBgnd.UpdateBgndStage2(dTime);
	else if (stage == Stages::STAGE3)		mBgnd.UpdateBgndStage3(dTime);

	mScore.mCoinSpin.GetAnim().Update(dTime);
	UpdateMissile(dTime);
	mPlayer.UpdateInput(dTime);
	mPlayer.UpdateThrust(dTime);
	updateAsteroids(dTime);
	if(stage != Stages::STAGE1 && stage != Stages::BOSS)	UpdateEnemy(dTime);
}

void PlayMode::BossModeUpdate(float dTime)
{
	audio.audio.Update();
	//update the background
	mBoss.UpdateBossBgnd(dTime);

	//update the input
	mPlayer.UpdateInput(dTime);

	//update the thrust
	mPlayer.UpdateThrust(dTime);

	//update the missile
	UpdateMissile(dTime);
	mBoss.updateMissile(dTime);

	//Update the boss movement
	mBoss.UpdateBoss(dTime);
	//update the coin spin
	mScore.mCoinSpin.GetAnim().Update(dTime);
}

void PlayMode::Render(float dTime, DirectX::SpriteBatch & batch) {
	//Changing background based on stage
	
	if (stage == Stages::STAGE1)	mBgnd.RenderBgndStage1(dTime, batch);
	else if (stage == Stages::STAGE2)	mBgnd.RenderBgndStage2(dTime, batch);
	else if (stage == Stages::STAGE3)	mBgnd.RenderBgndStage3(dTime, batch);

	if (mPlayer.mThrusting > GetClock())
		mPlayer.mThrust.Draw(batch);

	mScore.mCoinSpin.Draw(batch);
	stringstream ss;
	ss << std::setfill('0') << std::setw(4) << mScore.getScore();
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	if (stage != Stages::STAGE3 && stage != Stages::BOSS)
		mpFont->DrawString(&batch, ss.str().c_str(), Vector2(w*0.86f, h*0.9f), Vector4(0, 0, 0, 1));
	else
		mpFont->DrawString(&batch, ss.str().c_str(), Vector2(w*0.86f, h*0.9f), Vector4(1, 1, 1, 1));	//Changes to white so it doesnt blend in with the background on stage 3

	DirectX::XMVECTOR color;
	color = Colors::White;
	mMissile.Render(batch);
	mPlayer.mPlayer.Draw(batch);
	if (stage == Stages::BOSS)	mBoss.boss.Draw(batch);
	if (stage != Stages::BOSS)	renderAsteroids(batch);
	if (stage == Stages::STAGE2 || stage == Stages::STAGE3)  RenderEnemy(batch);
}

void PlayMode::Release()
{
	delete mpFont;
	mpFont = nullptr;
}

Asteroid *PlayMode::checkCollAsteroids(Asteroid& me)
{
	assert(!mAsteroids.empty());
	float radius = mAsteroids[0].spr.GetScreenSize().Length() / 2.f;
	size_t i = 0;
	Asteroid *pColl = nullptr;
	while (i < mAsteroids.size() && !pColl)
	{
		Asteroid& collider = mAsteroids[i];
		if ((&me != &collider) && collider.active && (collider.spr.mPos - me.spr.mPos).Length() < (radius * 2))
			pColl = &mAsteroids[i];
		i++;
	}
	return pColl;
}

Asteroid* PlayMode::spawnAsteroid()
{
	assert(!mAsteroids.empty());
	size_t i = 0;
	Asteroid*p = nullptr;
	while (i < mAsteroids.size() && !p)
	{
		if (!mAsteroids[i].active)
			p = &mAsteroids[i];
		i++;
	}

	if (p)
	{
		int w, h;
		WinUtil::Get().GetClientExtents(w, h);
		float radius = mAsteroids[0].spr.GetScreenSize().Length() / 2.f;
		Vector2& pos = p->spr.mPos;
		pos.y = (float)GetRandom(radius, h - radius);
		pos.x = (float)(w + radius);
		bool collision = false;
		if (checkCollAsteroids(*p))
			collision = true;
		if (!collision)
			p->active = true;
		else
			p = nullptr;
	}
	return p;
}

void PlayMode::updateAsteroids(float dTime)
{
	assert(!mAsteroids.empty());
	float radius = mAsteroids[0].spr.GetScreenSize().Length() / 2.f;
	for (auto& a : mAsteroids)
		a.Update(dTime);

	if (stage == Stages::STAGE1)		//Resets the asteroid spawn rate when restarting the game
		SpawnRateSec = 0.7f;

	if (stage == Stages::STAGE2)		//Changing the asteroid spawn rate to increase difficulty for stage 2
		SpawnRateSec = 0.4f;

	if (stage == Stages::STAGE3)		//Changing the asteroid spawn rate to increase difficulty for stage 3
		SpawnRateSec = 0.15f;

	if ((GetClock() - lastSpawn) > SpawnRateSec)
	{
		if (spawnAsteroid())
			lastSpawn = GetClock();
	}
	size_t i = 0;
	while (i < mAsteroids.size())
	{
		Asteroid& collider = mAsteroids[i];

		//Making the asteroids faster to increase difficulty for each stage
		if (stage == Stages::STAGE1)	collider.asteroid_speed = 100;
		if (stage == Stages::STAGE2)	collider.asteroid_speed = 200;
		if (stage == Stages::STAGE3)		collider.asteroid_speed = 250;

		//When an asteroid has collided with a missile
		if (collider.active && (collider.spr.mPos - mMissile.bullet.mPos).Length() < 30)
		{
			mScore.addToScore();		//Adds 40 to score
			asteroidKillTotal++;
			if (stage == Stages::STAGE2)	STG2totalKilled++;
			mMissile.active = false;
			mMissile.bullet.mPos = Vector2(-100, 100);
			collider.active = false;
			collider.spr.mPos = Vector2(100, -100);
			audio.audio.GetSfxMgr()->Play("smallExplosion", false, false, &audio.sfxHandle, 0.2f);
		}

		//When the player has collided with an asteroid
		if (collider.active && (collider.spr.mPos - mPlayer.mPlayer.mPos).Length() < 35)
		{
			mPlayer.hasDied = true;
			collider.active = false;
			collider.spr.mPos = Vector2(100, -100);
			audio.audio.GetSfxMgr()->Play("largeExplosion", false, false, &audio.sfxHandle, 0.2f);
			return;
		}
		i++;
	}

}

void PlayMode::initAsteroids()
{
	assert(mAsteroids.empty());
	Asteroid a(mD3D);
	a.Init();
	mAsteroids.insert(mAsteroids.begin(), GC::roid_cache, a);
	for (auto& a : mAsteroids)
	{
		if (GetRandom(0, 1) == 0)
			a.spr.GetAnim().Init(0, 31, GetRandom(10.f, 20.f), true);
		else
			a.spr.GetAnim().Init(32, 63, GetRandom(10.f, 20.f), true);

		a.spr.GetAnim().SetFrame(GetRandom(a.spr.GetAnim().GetStart(), a.spr.GetAnim().GetEnd()));
	}
}

void PlayMode::renderAsteroids(SpriteBatch & batch)
{
	for (auto& a : mAsteroids)
		a.Render(batch);
}

void PlayMode::InitEnemy()
{
	enemy.Init(mD3D);
}

void PlayMode::UpdateEnemy(float dTime)
{
	enemy.Update(dTime);

	if ((GetClock() - enemy.enemyLastSpawn) > enemy.enemySpawnRate)
	{
		if (spawnEnemy())
			enemy.enemyLastSpawn = GetClock();
	}


	//When the enemy is hit by a missile
	if (enemy.active && (enemy.enemySpr.mPos - mMissile.bullet.mPos).Length() < 45)
	{
		enemy.active = false;
		mScore.addToScore();
		asteroidKillTotal++;
		if (stage == Stages::STAGE2)	STG2totalKilled++;
		mMissile.active = false;
		mMissile.bullet.mPos = Vector2(-100, 100);
		enemy.enemySpr.mPos = Vector2(100, -100);
		audio.audio.GetSfxMgr()->Play("smallExplosion", false, false, &audio.sfxHandle, 0.2f);
	}

	//When the player has collided with the enemy
	if (enemy.active && (enemy.enemySpr.mPos - mPlayer.mPlayer.mPos).Length() < 50)
	{
		mPlayer.hasDied = true;
		enemy.active = false;
		enemy.enemySpr.mPos = Vector2(100, -100);
		audio.audio.GetSfxMgr()->Play("largeExplosion", false, false, &audio.sfxHandle, 0.2f);
		return;
	}
}

void PlayMode::RenderEnemy(DirectX::SpriteBatch& batch)
{
	enemy.Render(batch);
}

Enemy* PlayMode::spawnEnemy()
{
	Enemy*p = nullptr;
	p = &enemy;

	if (p)
	{
		int w, h;
		WinUtil::Get().GetClientExtents(w, h);
		float radius = enemy.enemySpr.GetScreenSize().Length() / 2.f;
		Vector2& pos = p->enemySpr.mPos;
		pos.y = (float)GetRandom(radius, h - radius);
		pos.x = (float)(w + radius);
		p->active = true;
	}

	return p;
}

void PlayMode::restartGame()
{
	mPlayer.hasDied = false;
	mScore.resetScore();
	mBoss.resetBossHealth();
	asteroidKillTotal = 0;
	totalMissilesFired = 0;
	mBgnd.SCROLL_SPEED = 250.f;

	//Resets positions of Asteroids
	mAsteroids.clear();
	initAsteroids();

	//Reset positions of Enemy ships
	enemy.active = false;
	InitEnemy();

	mMissile.active = false;		//Resets any missiles on screen when player died
	mBoss.mBossMissile.active = false;	//Resets any boss missiles on screen when player died
	mPlayer.mPlayer.mPos = Vector2(mPlayer.mPlayArea.left + mPlayer.mPlayer.GetScreenSize().x / 2.f, (mPlayer.mPlayArea.bottom - mPlayer.mPlayArea.top) / 2.f);	//Resets the position of the player when they die
}

void PlayMode::BossModeRender(float dTime, DirectX::SpriteBatch& batch)
{
	mBoss.RenderBossBgnd(dTime, batch);

	if (mPlayer.mThrusting > GetClock())
		mPlayer.mThrust.Draw(batch);

	mScore.mCoinSpin.Draw(batch);
	stringstream ss;
	ss << std::setfill('0') << std::setw(4) << mScore.getScore();
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, ss.str().c_str(), Vector2(w*0.86f, h*0.9f), Vector4(1, 1, 1, 1));	//Changes to white so it doesnt blend in with the background on stage 3

	DirectX::XMVECTOR color;
	color = Colors::White;
	mMissile.Render(batch);
	mBoss.mBossMissile.Render(batch);
	mPlayer.mPlayer.Draw(batch);
	mBoss.boss.Draw(batch);
}