#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__
#include "Levels.h"
struct GameOver : GameLevel{

	 LPD3DXSPRITE d3dspt;
	 LPD3DXFONT dxfont; 
	 bool win;
	 void Init();
     GameLevel* OnUpdate();
	 void HandleKeyboard();
	 void HandleStopKeyboard();
	 void HandleMouse(RAWINPUT MouseData);
     void Render(LPDIRECT3DDEVICE9& d3ddev);
     void OnLeave();
};

#endif