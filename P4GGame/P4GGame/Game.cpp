#include <iomanip>
#include <fstream>

#include "Game.h"
#include "WindowUtils.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

MouseAndKeys Game::sMKIn;
Gamepads Game::sGamepads;

Game::Game(MyD3D& d3d, DirectX::SpriteFont *mpFont)
	: mPMode(d3d), mD3D(d3d), mIntro(d3d, mpFont), mControls(d3d, mpFont), mHighScoreMenu(d3d, mpFont), mpFont(mpFont), mGoverMenu(d3d, mpFont), 
	mS1Clear(d3d, mpFont), mS2Clear(d3d, mpFont), mS3Clear(d3d, mpFont), mBossClear(d3d, mpFont)
{
	sMKIn.Initialise(WinUtil::Get().GetMainWnd(), true, false);
	sGamepads.Initialise();
	mpSB = new SpriteBatch(&mD3D.GetDeviceCtx());
}

void Game::Release()
{
	mIntro.Release();
	mPMode.Release();
	delete mpSB;
	mpSB = nullptr;
}

//called over and over, use it to update game logic
void Game::Update(float dTime, struct tm& startOfStage)
{
	//Easier way to see boss if too difficult.
	/*if (Game::sMKIn.IsPressed(VK_1))
	{
		state = State::Stage3ClearScreen;
	}*/

	if (state == State::IntroMode)
		checkIntroState();		//Checks state changes on the intro screen.

	if (state == State::GOverMode)
		checkOverState();

	checkPlayerDied();

	//Go back to the previous visited page E.g from controls back to intro menu
	if (Game::sMKIn.IsPressed(VK_B) && (state == State::Controls || state == State::HighScoreTable))
		state = lastState;

	checkStageClearState();		//Gets input when on a stage clear screen
	
	StageObjectiveComp();		//Checks if the objective for the current stage is complete


	if (state == State::BossClearScreen)
		checkEndState();
	
	sGamepads.Update();
	switch (state)
	{
	case State::PLAY:
		mPMode.Update(dTime);
		break;
	case State::GOverMode:
		//Show game over image with score then esc to quit
		mGoverMenu.gameoverMode();
		break;
	case State::IntroMode:
		//Show the introduction image then space to start game
		mIntro.introMode();
		break;
	case State::Controls:
		//function to render the control menu
		mControls.controlMode();
		break;
	case State::HighScoreTable:
		//function to show the player the highscore table
		mHighScoreMenu.highScoreMode();
		break;
	case State::Stage1ClearScreen:
		mS1Clear.stage1Clear();
		break;
	case State::Stage2ClearScreen:
		mS2Clear.stage2Clear();
		break;
	case State::Stage3ClearScreen:
		mS3Clear.stage3Clear();
		break;
	case State::BOSS:
		mPMode.BossModeUpdate(dTime);
		break;
	case State::BossClearScreen:
		mBossClear.bossClear();
		break;
	}
}

//called over and over, use it to render things
void Game::Render(float dTime)
{
	mD3D.BeginRender(Colours::Black);
	
	CommonStates dxstate(&mD3D.GetDevice());
	mpSB->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &mD3D.GetWrapSampler());

	int score = mPMode.mScore.getScore();

	switch (state)
	{
	case State::PLAY:
		mPMode.Render(dTime, *mpSB);
		break;
	case State::IntroMode:
		mIntro.introRender(*mpSB);
		break;
	case State::Controls:
		mControls.controlRender(*mpSB);
		break;
	case State::HighScoreTable:
		mHighScoreMenu.highScoreRender(*mpSB);
		break;
	case State::GOverMode:
		mGoverMenu.gameoverRender(*mpSB, score);
		break;
	case State::Stage1ClearScreen:
		mS1Clear.stage1ClearRender(*mpSB, mPMode.totalMissilesFired, mPMode.asteroidKillTotal, score);
		break;
	case State::Stage2ClearScreen:
		mS2Clear.stage2ClearRender(*mpSB, mPMode.totalMissilesFired, mPMode.asteroidKillTotal, score);
		break;
	case State::Stage3ClearScreen:
		mS3Clear.stage3ClearRender(*mpSB, mPMode.totalMissilesFired, mPMode.asteroidKillTotal, score);
		break;
	case State::BOSS:
		mPMode.BossModeRender(dTime, *mpSB);
		break;
	case State::BossClearScreen:
		mBossClear.bossClearRender(*mpSB, score);
		break;
	}

	mpSB->End();

	mD3D.EndRender();
	sMKIn.PostProcess();
}

void Game::checkIntroState()
{
	//Starting the game by pressing S
	if (Game::sMKIn.IsPressed(VK_S))
	{
		mPMode.audio.audio.GetSongMgr()->Stop();
		state = State::PLAY;
		mPMode.stage = Stages::STAGE1;
		now = time(0);
		startOfStage = *localtime(&now);
		startOfStageMins = startOfStage.tm_min;
		mPMode.audio.audio.GetSongMgr()->Play("stage1", true, false, &mPMode.audio.sfxHandle, 0.4f);
	}

	//View the highscore table from the intro screen
	if (Game::sMKIn.IsPressed(VK_H))
	{
		state = State::HighScoreTable;
		lastState = State::IntroMode;
	}

	//View the controls of the game
	if (Game::sMKIn.IsPressed(VK_C))
	{
		state = State::Controls;
		lastState = State::IntroMode;
	}
}

void Game::checkStageClearState()
{
	//Start Stage 2 when on the stage 1 clear screen
	if (state == State::Stage1ClearScreen)
	{
		if (Game::sMKIn.IsPressed(VK_S))
		{
			mPMode.stage = Stages::STAGE2;
			state = State::PLAY;
			mPMode.asteroidKillTotal = 0;
			mPMode.totalMissilesFired = 0;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("stage2", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}

	else if (state == State::Stage2ClearScreen)
	{
		//Start Stage 3 when on the stage 2 clear screen
		if (Game::sMKIn.IsPressed(VK_S))
		{
			mPMode.stage = Stages::STAGE3;
			state = State::PLAY;
			mPMode.asteroidKillTotal = 0;
			mPMode.totalMissilesFired = 0;
			now = time(0);
			startOfStage = *localtime(&now);
			startOfStageMins = startOfStage.tm_min;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("stage3", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}

	else if (state == State::Stage3ClearScreen)
	{
		//Starts the BOSS stage when on the stage 3 clear screen
		if (Game::sMKIn.IsPressed(VK_S))
		{
			mPMode.stage = Stages::BOSS;
			state = State::BOSS;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("boss", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}
}

void Game::StageObjectiveComp()
{
	if (mPMode.stage == Stages::STAGE1)
	{
		//Gets the total time that has passed since the player started stage 1
		now = time(0);
		currentTime = *localtime(&now);
		currentMins = currentTime.tm_min;

		//Objective for Stage 1 (Survive for time between range 1m 01s or 2m 00s)
		if (currentMins >= (startOfStageMins + 2)&& state == State::PLAY)
		{
			state = State::Stage1ClearScreen;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("inbetweenScreenTheme", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}

	else if (state == State::PLAY && mPMode.stage == Stages::STAGE2)
	{
		if (mPMode.STG2totalKilled > 59)
		{
			//If the objective has been met in Stage 2 (Objective is 60 enemies killed)
			state = State::Stage2ClearScreen;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("inbetweenScreenTheme", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}

	else if (mPMode.stage == Stages::STAGE3)
	{
		now = time(0);			//Gets the total time that the player has been on stage 3
		currentTime = *localtime(&now);
		currentMins = currentTime.tm_min;

		//Objective for Stage 3 (Survive without shooting for range between 1m 01s or 2m 00s
		if (currentMins >= (startOfStageMins + 2) && state == State::PLAY)
		{
			state = State::Stage3ClearScreen;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("inbetweenScreenTheme", true, false, &mPMode.audio.sfxHandle, 0.4f);
		}
	}

	else if (state == State::BOSS)
	{
		//When the boss has been defeated
		if (mPMode.mBoss.getBossHealth() < 0)
		{
			state = State::BossClearScreen;
			mPMode.audio.audio.GetSongMgr()->Stop();
			mPMode.audio.audio.GetSongMgr()->Play("endScreen", true, false, &mPMode.audio.sfxHandle, 0.4f);

			storeScore();
		}
	}
}

void Game::checkOverState()
{
	//View the highscore table from the gameover screen
	if (Game::sMKIn.IsPressed(VK_H))
	{
		state = State::HighScoreTable;
		lastState = State::GOverMode;
	}

	//When the player resets the game when they have a gameover
	if (Game::sMKIn.IsPressed(VK_R))
	{
		mPMode.restartGame();
		state = State::IntroMode;
		mPMode.audio.audio.GetSongMgr()->Stop();
		mPMode.audio.audio.GetSongMgr()->Play("menuTheme", true, false, &mPMode.audio.sfxHandle, 0.4f);
	}
}

void Game::checkEndState()
{
	//View the highscore table from the Boss clear screen
	if (Game::sMKIn.IsPressed(VK_H))
	{
		state = State::HighScoreTable;
		lastState = State::BossClearScreen;
	}

	//Reset the game from the boss clear screen
	if (Game::sMKIn.IsPressed(VK_R))
	{
		mPMode.restartGame();
		state = State::IntroMode;
		mPMode.audio.audio.GetSongMgr()->Stop();
		mPMode.audio.audio.GetSongMgr()->Play("menuTheme", true, false, &mPMode.audio.sfxHandle, 0.4f);
	}
}

void Game::checkPlayerDied()
{
	//If the player has died
	if (mPMode.mPlayer.playerDied() && (state == State::PLAY || state == State::BOSS))
	{
		mPMode.audio.audio.GetSongMgr()->Stop();
		state = State::GOverMode;
		mPMode.audio.audio.GetSongMgr()->Play("gameover", true, false, &mPMode.audio.sfxHandle, 0.4f);
		storeScore();
	}
}

void Game::storeScore()
{
	vector<int> scores(5);
	//Stores score to txt file to be used in highscore table
	ifstream fin;
	fin.open("scores.txt", ios::out);
	if (fin.fail())
	{
		ofstream fout("scores.txt");
		if (fout.fail())
			assert(false);
		else
		{
			fout << mPMode.mScore.getScore();
			fout.close();
		}
	}
	else
	{
		int previousScore;
		fin >> scores.at(0) >> scores.at(1) >> scores.at(2) >> scores.at(3) >> scores.at(4);
		ofstream fout("scores.txt", ios::out);
		if (fout.fail())
			assert(false);
		else
		{

			for (int i = 4; i > -1; i--)
			{
				if (mPMode.mScore.getScore() >= scores.at(i))
				{
					previousScore = scores.at(i);
					scores.at(i) = mPMode.mScore.getScore();		//Replace score at 5th position with new score
					if (i < 4)
						scores.at(i + 1) = previousScore;
				}

			}


			fout << scores.at(0) << "\n" << scores.at(1) << "\n" << scores.at(2) << "\n" << scores.at(3) << "\n" << scores.at(4) << "\n";
			fout.close();
		}
	}
}