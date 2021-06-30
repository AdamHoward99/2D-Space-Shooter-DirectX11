#pragma once

#include <vector>
#include <ctime>

#include "Input.h"
#include "D3D.h"
#include "SpriteBatch.h"
#include "Sprite.h"
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "AudioMgrFMOD.h"

#include "Asteroid.h"
#include "Enemy.h"
#include "BossBullet.h"
#include "Bullet.h"
#include "Sound.h"
#include "Player.h"
#include "Score.h"
#include "Backgrounds.h"
#include "BossMode.h"

enum class Stages { STAGE1, STAGE2, STAGE3, BOSS };
class PlayMode
{
public:
	PlayMode(MyD3D& d3d);
	void Update(float dTime);
	void Render(float dTime, DirectX::SpriteBatch& batch);
	void Release();

	void BossModeUpdate(float dTime);
	void BossModeRender(float dTime, DirectX::SpriteBatch& batch);

	void restartGame();
	Stages stage;
	int STG2totalKilled = 0;

	//Stat variables
	float asteroidKillTotal = 0.f;
	float totalMissilesFired = 0.f;

	//Music and SFX for game
	Sound audio;

	Score mScore;

	BossMode mBoss;
	Player mPlayer;
private:
	MyD3D& mD3D;

	Bullet mMissile;	//weapon, only one at once
	Enemy enemy;
	Backgrounds mBgnd;

	//Asteroid Stuff
	std::vector<Asteroid> mAsteroids;
	float SpawnRateSec = GC::asteroid_spawn_rate;
	float lastSpawn = 0;
	Asteroid* spawnAsteroid();
	void updateAsteroids(float dTime);
	void initAsteroids();
	void renderAsteroids(DirectX::SpriteBatch & batch);
	Asteroid *checkCollAsteroids(Asteroid& me);

	DirectX::SpriteFont *mpFont = nullptr;

	//Enemy type 2 stuff
	void InitEnemy();
	void UpdateEnemy(float dTime);
	void RenderEnemy(DirectX::SpriteBatch & batch);
	Enemy* spawnEnemy();

	void UpdateMissile(float dTime);
};