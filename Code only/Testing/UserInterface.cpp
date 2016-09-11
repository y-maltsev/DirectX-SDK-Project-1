#include "UserInterface.h"
#include "Game.h"
#include "Player.h"
#include "GlobalIncludes.h"
#include <sstream>
#include <string>
UserInterface::UserInterface(LPDIRECT3DDEVICE9& d3ddev){
	D3DXCreateSprite(d3ddev, &d3dspt);
   D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "Media/Soldier_lifes.PNG",    // the new file name
                                D3DX_DEFAULT,    // default width
                                D3DX_DEFAULT,    // default height
                                D3DX_DEFAULT,    // no mip mapping
                                NULL,    // regular usage
                                D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
                                D3DPOOL_MANAGED,    // typical memory handling
                                D3DX_DEFAULT,    // no filtering
                                D3DX_DEFAULT,    // no mip filtering
                                D3DCOLOR_XRGB(0, 0, 0),    // the black color key
                                NULL,    // no image info struct
                                NULL,    // not using 256 colors
                                &life);    // load to sprite
    D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "Media/heart.png",    // the new file name
                                D3DX_DEFAULT,    // default width
                                D3DX_DEFAULT,    // default height
                                D3DX_DEFAULT,    // no mip mapping
                                NULL,    // regular usage
                                D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
                                D3DPOOL_MANAGED,    // typical memory handling
                                D3DX_DEFAULT,    // no filtering
                                D3DX_DEFAULT,    // no mip filtering
                                D3DCOLOR_XRGB(0, 0, 0),    // the white color key
                                NULL,    // no image info struct
                                NULL,    // not using 256 colors
                                &health);    // load to sprite
	 D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
                                "Media/Target.bmp",    // the new file name
                                D3DX_DEFAULT,    // default width
                                D3DX_DEFAULT,    // default height
                                D3DX_DEFAULT,    // no mip mapping
                                NULL,    // regular usage
                                D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
                                D3DPOOL_MANAGED,    // typical memory handling
                                D3DX_DEFAULT,    // no filtering
                                D3DX_DEFAULT,    // no mip filtering
                                D3DCOLOR_XRGB(0, 0, 0),    // the black color key
                                NULL,    // no image info struct
                                NULL,    // not using 256 colors
                                &target);    // load to sprite
		 D3DXCreateFont(d3ddev,    // the D3D Device
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

void UserInterface::Draw(){
	static RECT textbox;
	D3DXVECTOR3 position;
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
	stringstream prebuffer;
	string buffer;

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency
	if(Game::player!=NULL){
		//Draw target cursor
		position =D3DXVECTOR3( SCREEN_WIDTH/2 - 30 ,  SCREEN_HEIGHT/2 - 40 , 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(target, NULL, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
		//Draw lifes
		 SetRect(&textbox, 0, 0, 70, 1670); 
		dxfont->DrawTextA(NULL,"Lives:",6,&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		for(int i=0;i<Player::lifes;i++){
		position = D3DXVECTOR3(60.0f *i +60.0f, 800.0f, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(life, NULL, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		//Draw health
		SetRect(&textbox, 0, 0, 80, 1770); 
		dxfont->DrawTextA(NULL,"Health:",7,&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
		for(int i=0;i<Game::player->health/25;i++){
			position= D3DXVECTOR3(30.0f *i +80.0f, 870.0f, 0.0f);    
			d3dspt->Draw(health, NULL, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		prebuffer <<"Score: "<< Game::player->Score;
		buffer = prebuffer.str();
		prebuffer.str("");
		SetRect(&textbox, 0, 0, 3050, 50); 
		dxfont->DrawTextA(NULL,buffer.c_str(),buffer.size(),&textbox,DT_CENTER | DT_VCENTER,D3DCOLOR_ARGB(255, 255, 255, 255));
	}
    d3dspt->End();    // end sprite drawing
}