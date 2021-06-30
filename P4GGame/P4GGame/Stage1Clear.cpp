#include "Stage1Clear.h"
#include <vector>
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Stage1Clear::stage1Clear()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));

	p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Stage1Title.dds", "stage1Title", true);
	stage1Title.SetTex(*p);
	stage1Title.SetScale(Vector2(0.4f, 0.4f));
	stage1Title.mPos = Vector2(230.f, 80.f);
}

void Stage1Clear::stage1ClearRender(DirectX::SpriteBatch& batch, float totalMissilesFired, float asteroidKillTotal, int score)
{
	float accuracy = (asteroidKillTotal / totalMissilesFired) * 100;

	intro.Draw(batch);
	stage1Title.Draw(batch);
	stringstream txt;
	txt << "Score: " << score << "\nAsteroids Destroyed : " << asteroidKillTotal << "\nMissiles Fired : " << totalMissilesFired << "\nAccuracy Rate: " << accuracy << " %" << "\nNext Objective: Defeat 60 enemies\nGood Luck"
		<< "\nPress S to continue to Stage 2";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.3f, h*0.3f), Vector4(1, 1, 1, 1));
}