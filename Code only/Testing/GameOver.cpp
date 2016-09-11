
#include "MainMenu.h"
#include "Player.h"
#include "Game.h"
#include "GameOver.h"
#include <sstream>
#include <string>
#define KEY_DOWN(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 0 : 1)


void GameOver::Init(){
	D3DXCreateSprite(Game::d3ddev, &d3dspt);
	D3DXCreateFont(Game::d3ddev,    // the D3D Device
                   30,    // font height of 30
                   0,    // default font width
                   FW_NORMAL,    // font weight
                   1,    // not using MipLevels
                   true,    // italic font
                   DEFAULT_CHARSET,    // default character set
                   OUT_DEFAULT_PRECIS,    // default OutputPrecision,
                   DEFAULT_QUALITY,    // default Quality
                   DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
                   "Arial",    // use Facename Arial
                   &dxfont);    // the font object	
}

GameLevel* GameOver::OnUpdate(){
	return this;
}

void GameOver::HandleKeyboard(){
	extern Game game;
	game.HandleKeyboard();
}
void GameOver::HandleStopKeyboard(){}
void GameOver::HandleMouse(RAWINPUT MouseData){}


void GameOver::Render(LPDIRECT3DDEVICE9& d3ddev){
	
		d3dspt->Begin(D3DXSPRITE_ALPHABLEND); 

		static RECT textbox;
		stringstream prebuffer;
		string buffer;

		prebuffer <<"GAME OVER";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 900); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		
		
		if(win)
		prebuffer <<"YOU WIN";
		else prebuffer <<"YOU LOOSE";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 1000); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		
		prebuffer <<"Total Score:"<<Game::player->Score;
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 1100); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 127, 255, 127));
		d3dspt->End(); 

		prebuffer <<"(press Esc to enter the main menu)";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 1200); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(127, 255, 255, 255));
		d3dspt->End(); 
	
}
void GameOver::OnLeave(){}
