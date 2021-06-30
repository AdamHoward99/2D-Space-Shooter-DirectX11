#include "Player.h"
#include "WindowUtils.h"
#include "Game.h"
#include <vector>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Player::InitPlayer()
{
	ID3D11ShaderResourceView *p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "ship.dds");
	mPlayer.SetTex(*p);
	mPlayer.SetScale(Vector2(0.5f, 0.5f));
	mPlayer.origin = mPlayer.GetTexData().dim / 2.f;
	mPlayer.rotation = PI / 2.f;

	//setup the play area
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mPlayArea.left = mPlayer.GetScreenSize().x*0.6f;
	mPlayArea.top = mPlayer.GetScreenSize().y * 0.6f;
	mPlayArea.right = w - mPlayArea.left;
	mPlayArea.bottom = h * 0.85f;
	mPlayer.mPos = Vector2(mPlayArea.left + mPlayer.GetScreenSize().x / 2.f, (mPlayArea.bottom - mPlayArea.top) / 2.f);

	vector<RECTF> frames(thrustAnim, thrustAnim + sizeof(thrustAnim) / sizeof(thrustAnim[0]));
	p = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "thrust.dds", "thrust", true, &frames);
	mThrust.SetTex(*p);
	mThrust.GetAnim().Init(0, 3, 15, true);
	mThrust.GetAnim().Play(true);
	mThrust.rotation = PI / 2.f;
}

void Player::UpdateInput(float dTime)
{
	Vector2 mouse{ Game::sMKIn.GetMousePos(false) };
	bool keypressed = Game::sMKIn.IsPressed(VK_UP) || Game::sMKIn.IsPressed(VK_DOWN) ||
		Game::sMKIn.IsPressed(VK_RIGHT) || Game::sMKIn.IsPressed(VK_LEFT);
	bool sticked = false;
	if (Game::sGamepads.IsConnected(0) &&
		(Game::sGamepads.GetState(0).leftStickX != 0 || Game::sGamepads.GetState(0).leftStickX != 0))
		sticked = true;

	if (keypressed || (mouse.Length() > VERY_SMALL) || sticked)
	{
		//move the ship around
		Vector2 pos(0, 0);
		if (Game::sMKIn.IsPressed(VK_UP))
			pos.y -= SPEED * dTime;
		else if (Game::sMKIn.IsPressed(VK_DOWN))
			pos.y += SPEED * dTime;
		if (Game::sMKIn.IsPressed(VK_RIGHT))
			pos.x += SPEED * dTime;
		else if (Game::sMKIn.IsPressed(VK_LEFT))
			pos.x -= SPEED * dTime;

		pos += mouse * MOUSE_SPEED * dTime;

		if (sticked)
		{
			DBOUT("left stick x=" << Game::sGamepads.GetState(0).leftStickX << " y=" << Game::sGamepads.GetState(0).leftStickY);
			pos.x += Game::sGamepads.GetState(0).leftStickX * PAD_SPEED * dTime;
			pos.y -= Game::sGamepads.GetState(0).leftStickY * PAD_SPEED * dTime;
		}

		//keep it within the play area
		pos += mPlayer.mPos;
		if (pos.x < mPlayArea.left)
			pos.x = mPlayArea.left;
		else if (pos.x > mPlayArea.right)
			pos.x = mPlayArea.right;
		if (pos.y < mPlayArea.top)
			pos.y = mPlayArea.top;
		else if (pos.y > mPlayArea.bottom)
			pos.y = mPlayArea.bottom;

		mPlayer.mPos = pos;
		mThrusting = GetClock() + 0.2f;
	}
}

void Player::UpdateThrust(float dTime)
{
	if (mThrusting)
	{
		mThrust.mPos = mPlayer.mPos;
		mThrust.mPos.x -= 30;
		mThrust.mPos.y -= 12;
		mThrust.SetScale(Vector2(1.5f, 1.5f));
		mThrust.GetAnim().Update(dTime);
	}
}

bool Player::playerDied()
{
	return hasDied;
}