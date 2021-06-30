#include "Intro.h"
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


void Intro::introMode()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));

	p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "TitleImage.dds", "title", true);
	title.SetTex(*p);
	title.SetScale(Vector2(0.75f, 0.75f));
	title.mPos = Vector2(200.f, 120.f);
}

void Intro::introRender(DirectX::SpriteBatch& batch)
{
	intro.Draw(batch);
	title.Draw(batch);

	stringstream txt, txt2, txt3, txt4;
	txt << "Press S to start the game";
	txt2 << "Press H to view highscore table";
	txt3 << "Press C to view the controls";
	txt4 << "Press Escape to exit the game";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.25f, h*0.5f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt2.str().c_str(), Vector2(w*0.25f, h*0.6f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt3.str().c_str(), Vector2(w*0.25f, h*0.7f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt4.str().c_str(), Vector2(w*0.25f, h*0.8f), Vector4(1, 1, 1, 1));
	DirectX::XMVECTOR color;
	color = Colors::Blue;
}

void Intro::Release()
{
	delete mpFont;
	mpFont = nullptr;
}