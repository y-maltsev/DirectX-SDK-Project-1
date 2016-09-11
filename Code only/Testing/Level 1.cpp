#include "Level_1.h"
#include "Level_2.h"
#include "Game.h"
#include <sstream>
#include <string>
#include "GlobalIncludes.h"
void Level1::Init(){
	loading_time = 6;
	level_change_time=-1;
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
	D3DLIGHT9 light;    // create the light struct
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(-1.0f, -1.5f,-1.0f); 
	
	Game::d3ddev->SetLight(0, &light);    // send the light struct properties to light 
	
}



GameLevel* Level1::OnUpdate(){
	if(loading_time!=0){
		if(loading_time<6){
		D3DXVECTOR3 enemy_pos;
		enemy_pos = D3DXVECTOR3(rand()%54+1, 10.0f, rand()%54+1); 
		Enemy_1* enemy = new Enemy_1(Game::d3ddev,enemy_pos);
		Game::enemyes.push_back(enemy);
		}
		loading_time--;
	}
	else{
		if(Game::player!=NULL)
		{
			if(Game::camera->attached)Game::player->Rotate+= (Game::camera->RotateY - Game::player->Rotate);
			//else Game::player->Rotate+= Game::camera->movingY;
			Game::player->Update(Game::camera->C_Position,Game::camera->C_LookAtPos);
		}
		for(unsigned int i=0;i<Game::enemyes.size();i++)
			Game::enemyes[i]->Update();
		Game::camera->Update(Game::d3ddev);

		if (Game::enemyes.size()==0){
			if(level_change_time==-1)level_change_time=75;
			level_change_time--;
			if(level_change_time==0){
			Level2* ret = new Level2();
			return ret;
			}
		}
	}
	return this;
}

void Level1::HandleKeyboard(){
	extern Game game;
	game.HandleKeyboard();
}
void Level1::HandleMouse(RAWINPUT MouseData){
	extern Game game;
	game.HandleMouse(MouseData);
}

void Level1::Render(LPDIRECT3DDEVICE9& d3ddev){
	if(loading_time!=0){
		d3dspt->Begin(D3DXSPRITE_ALPHABLEND); 
		static RECT textbox;
		stringstream prebuffer;
		string buffer;
		float percentage_left = (10.0f-loading_time)/10.0f * 100.0f;
		prebuffer <<"LOADING...";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1550, 1000); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		d3dspt->End(); 
	}
	else{
		Game::terrain->Draw(d3ddev);
		if(Game::player!=NULL)
			Game::player->Draw(d3ddev);
		for(unsigned int i=0; i<Game::enemyes.size();i++)
			Game::enemyes[i]->Draw(d3ddev);
		Game::skybox->Draw(d3ddev);
		Game::UI->Draw();

		if(level_change_time>0){
			d3dspt->Begin(D3DXSPRITE_ALPHABLEND); 

		static RECT textbox;
		stringstream prebuffer;
		string buffer;
		float percentage_left = (10.0f-loading_time)/10.0f * 100.0f;
		prebuffer <<"MISSION COMPLETE. Going for next mission.";
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 1650, 750); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 0, 0));

		d3dspt->End(); 
		}
		else 
		if(Game::player!=NULL){
			d3dspt->Begin(D3DXSPRITE_ALPHABLEND); 
			static RECT textbox;
			stringstream prebuffer;
			string buffer;
			prebuffer <<"Enemies:"<<Game::enemyes.size()<<"/5";
			buffer = prebuffer.str();
			prebuffer.str("");
			SetRect(&textbox, 0, 0, 1650, 50); 
			dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));

			d3dspt->End(); 
		}
	}
}
void Level1::OnLeave(){}
