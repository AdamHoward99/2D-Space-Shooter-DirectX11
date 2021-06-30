#include "GameOver.h"
#include "WindowUtils.h"
#include <vector>
#include <iomanip>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void GameOver::gameoverMode()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "gameOver.dds", "gameover", true);
	gameover.SetTex(*p);
	gameover.SetScale(Vector2(WinUtil::Get().GetClientWidth() / gameover.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / gameover.GetTexData().dim.y));

	p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "GameOverTitle.dds", "gameoverTitle", true);
	gameoverTitle.SetTex(*p);
	gameoverTitle.SetScale(Vector2(0.75f, 0.75f));
	gameoverTitle.mPos = Vector2(150.f, 100.f);
}

void GameOver::gameoverRender(DirectX::SpriteBatch& batch, int score)
{
	gameover.Draw(batch);
	gameoverTitle.Draw(batch);
	stringstream ss, txt;
	ss << "Your score was: " << std::setfill('0') << std::setw(4) << score;
	txt << "Press H to view the highscore table\n\nPress R to restart the game";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, ss.str().c_str(), Vector2(w*0.35f, h*0.3f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.35f, h*0.4f), Vector4(1, 1, 1, 1));
}