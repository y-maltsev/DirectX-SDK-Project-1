#ifndef __MAINMENU_H__
#define __MAINMENU_H__
#include "Levels.h"
struct MainMenu : GameLevel{
	GameLevel* savelevel;
	int text_selected;
	 LPD3DXSPRITE d3dspt;
	 LPD3DXFONT dxfont; 

	 void Init();
     GameLevel* OnUpdate();
	 void HandleKeyboard();
	 void HandleStopKeyboard();
	 void HandleMouse(RAWINPUT MouseData);
     void Render(LPDIRECT3DDEVICE9& d3ddev);
     void OnLeave();
};

#endif