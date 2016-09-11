#ifndef __Level_1_H__
#define __Level_1_H__
#include "Levels.h"
struct Level1 : GameLevel{
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