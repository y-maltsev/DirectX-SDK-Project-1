
#include "MainMenu.h"
#include "Player.h"
#include "Game.h"
#include "Level_1.h"
#include "Player.h"
#include <sstream>
#include <string>
#define KEY_DOWN(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 0 : 1)
extern Game game;

void MainMenu::Init(){
	text_selected=1;
	savelevel=NULL;
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



GameLevel* MainMenu::OnUpdate(){
	return this;
}
void MainMenu::HandleKeyboard(){
	if(KEY_DOWN(0x0D)){
		if (text_selected==1){
			if(!savelevel==NULL)
			LevelManager::currentLevel = savelevel;
			delete this;
			return;
		}

		if(text_selected==2)
		{
			while(Game::enemyes.size()){
				Game::enemyes[0]->health=0;
				Game::enemyes[0]->Update();
			}

			delete Game::player;
			Game::player = new Player(Game::d3ddev);
			Game::camera->Attach(Game::player);

			GameLevel* level1 = new Level1(); 
			level1->Init();
			LevelManager::currentLevel = level1;
			delete this;
			return;
		}

		if(text_selected==3){
			PostQuitMessage(0);
		}
	}
	if(KEY_DOWN(0x28))
	{
		text_selected++;
		if(text_selected>3)text_selected=1;
	}
	if(KEY_DOWN(0x26))
	{
		text_selected--;
		if(text_selected<1)text_selected=3;
	}
}
void MainMenu::HandleMouse(RAWINPUT MouseData){}


void MainMenu::Render(LPDIRECT3DDEVICE9& d3ddev){
	
		d3dspt->Begin(D3DXSPRITE_ALPHABLEND); 

		static RECT textbox;
		stringstream prebuffer;
		string buffer;

		prebuffer <<"Resume Game";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 850); 
		if(text_selected==1)
			dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 127, 255, 127));
		else dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));

		prebuffer <<"New Game";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 950); 
		if(text_selected==2)
			dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 127, 255, 127));
		else dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));

		prebuffer <<"Exit";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 1050); 
		if(text_selected==3)
			dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 127, 255, 127));
		else dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		d3dspt->End(); 
	
}
void MainMenu::OnLeave(){}
