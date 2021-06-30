#include "ControlMenu.h"
#include "WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void ControlMenu::controlMode()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));
}

void ControlMenu::controlRender(DirectX::SpriteBatch& batch)
{
	intro.Draw(batch);
	stringstream txt, txt2, txt3;
	txt << "Use the arrow keys to move the ship around the screen";
	txt2 << "Use the space key to fire a missile towards the enemies";
	txt3 << "Press 'B' to go back";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.2f, h*0.3f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt2.str().c_str(), Vector2(w*0.2f, h*0.4f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, txt3.str().c_str(), Vector2(w*0.2f, h*0.5f), Vector4(1, 1, 1, 1));
}

void ControlMenu::Release()
{
	delete mpFont;
	mpFont = nullptr;
}