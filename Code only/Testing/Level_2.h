#ifndef __Level_2_H__
#define __Level_2_H__
#include "Levels.h"
struct Level2 : GameLevel{
	 LPD3DXSPRITE d3dspt;
	 LPD3DXFONT dxfont; 
	 int loading_time;
	 int level_change_time;
	 void Init();
     GameLevel* OnUpdate();
	 void HandleKeyboard();
	 void HandleMouse(RAWINPUT MouseData);
     void Render(LPDIRECT3DDEVICE9& d3ddev);
     void OnLeave();
};

#endif