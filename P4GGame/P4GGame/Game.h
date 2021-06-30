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

#include "PlayMode.h"
#include "Intro.h"
#include "ControlMenu.h"
#include "HighScore.h"
#include "GameOver.h"
#include "Stage1Clear.h"
#include "Stage2Clear.h"
#include "Stage3Clear.h"
#include "BossClear.h"
#include "State.h"

class Game
{
public:
	static MouseAndKeys sMKIn;
	static Gamepads sGamepads;
	State state = State::IntroMode;
	State lastState;
	Game(MyD3D& d3d, DirectX::SpriteFont *mpFont);

	void Release();
	void Update(float dTime, struct tm& startOfStage);
	void Render(float dTime);

	time_t now = time(0);
	struct tm& startOfStage = *localtime(&now);
	struct tm& currentTime = *localtime(&now);
	int currentMins;
	int startOfStageMins;

private:
	MyD3D& mD3D;
	DirectX::SpriteBatch *mpSB = nullptr;
	DirectX::SpriteFont *mpFont;
		
	PlayMode mPMode;
	Intro mIntro;
	ControlMenu mControls;
	HighScoreMenu mHighScoreMenu;
	GameOver mGoverMenu;
	Stage1Clear mS1Clear;
	Stage2Clear mS2Clear;
	Stage3Clear mS3Clear;
	BossClear mBossClear;

	void checkIntroState();
	void checkOverState();
	void checkEndState();
	void checkStageClearState();
	void StageObjectiveComp();
	void checkPlayerDied();

	void storeScore();
};


