#include "BossClear.h"
#include <vector>
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void BossClear::bossClear()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));

	p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "EndTitle.dds", "EndTitle", true);
	EndTitle.SetTex(*p);
	EndTitle.SetScale(Vector2(0.6f, 0.6f));
	EndTitle.mPos = Vector2(180.f, 80.f);
}

void BossClear::bossClearRender(DirectX::SpriteBatch& batch, int score)
{
	intro.Draw(batch);
	EndTitle.Draw(batch);
	stringstream txt;
	txt << "Your Final Score Was " << score << "\nYou saved the universe from darkness \nAnd thus life goes on\n Rest, Our Hero"
		<< "\n\nPress H to view the highscore table\nPress R to restart the game";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.3f, h*0.3f), Vector4(1, 1, 1, 1));
}