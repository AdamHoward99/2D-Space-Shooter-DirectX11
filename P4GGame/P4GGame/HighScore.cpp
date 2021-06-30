#include "HighScore.h"
#include "WindowUtils.h"
#include <fstream>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void HighScoreMenu::highScoreMode()
{
	ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "Intro.dds", "intro", true);
	intro.SetTex(*p);
	intro.SetScale(Vector2(WinUtil::Get().GetClientWidth() / intro.GetTexData().dim.x, WinUtil::Get().GetClientHeight() / intro.GetTexData().dim.y));
}

void HighScoreMenu::highScoreRender(DirectX::SpriteBatch& batch)
{
	intro.Draw(batch);
	stringstream txt, highscoreTb;
	txt << "High Score Table";
	vector<int> scores(5);

	//Get scores from scores.txt file
	ifstream fin;
	fin.open("scores.txt", ios::out);
	if (fin.fail())
		highscoreTb << "Error loading scores";
	else
		fin >> scores.at(0) >> scores.at(1) >> scores.at(2) >> scores.at(3) >> scores.at(4);

	fin.close();

	highscoreTb << "1st : " << scores.at(0)
		<< "\n2nd : " << scores.at(1)
		<< "\n3rd : " << scores.at(2)
		<< "\n4th : " << scores.at(3)
		<< "\n5th : " << scores.at(4) << "\n\nPress 'B' to go back";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mpFont->DrawString(&batch, txt.str().c_str(), Vector2(w*0.4f, h*0.3f), Vector4(1, 1, 1, 1));
	mpFont->DrawString(&batch, highscoreTb.str().c_str(), Vector2(w*0.4f, h* 0.4f), Vector4(1, 1, 1, 1));
}

void HighScoreMenu::Release()
{
	delete mpFont;
	mpFont = nullptr;
}