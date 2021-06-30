#include "Stage2Clear.h"
#include <vector>
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Stage2Clear::stage2Clear()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));

	p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "stage2Title.dds", "stage2Title", true);
	stage2Title.SetTex(*p);
	stage2Title.SetScale(Vector2(0.4f, 0.4f));
	stage2Title.mPos = Vector2(230.f, 80.f);
}

void Stage2Clear::stage2ClearRender(DirectX::SpriteBatch& batch, float totalMissilesFired, float asteroidKillTotal, int score)
{
	float accuracy = (asteroidKillTotal / totalMissilesFired) * 100;
	intro.Draw(batch);
	stage2Title.Draw(batch);
	stringstream txt;
	txt << "Score: " << score << "\nAsteroids Destroyed : " << asteroidKillTotal << "\nMissiles Fired : " << totalMissilesFired << "\nAccuracy Rate: " << accuracy << " %" << "\nNext Objective: Your weapons are temporarily disabled\nSurvive without shooting\nGood Luck"
		<< "\nPress S to continue to Stage 3";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.3f, h*0.3f), Vector4(1, 1, 1, 1));
}