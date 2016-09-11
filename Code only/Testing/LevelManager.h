#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__
#include "Globalincludes.h"
#include "Levels.h"
struct LevelManager
{
    
    static GameLevel* currentLevel;
    bool Update();
    void Render(LPDIRECT3DDEVICE9& d3ddev);
	void HandleKeyboard();
	
	void HandleMouse(RAWINPUT MouseData);
    void ChangeLevel(GameLevel* Level);
};

#endif