#include "LevelManager.h"
#include "Levels.h"

GameLevel* LevelManager::currentLevel;
bool LevelManager::Update()
{

    GameLevel* nextLevel = LevelManager::currentLevel->OnUpdate();
    if(nextLevel == NULL)
        return false;
    if(nextLevel != currentLevel)
        ChangeLevel(nextLevel);
    return true;
}

void LevelManager::ChangeLevel(GameLevel* level)
{
    currentLevel->OnLeave();
    delete currentLevel;
    currentLevel = level;
    currentLevel->Init();

}

void LevelManager::HandleKeyboard(){
	currentLevel->HandleKeyboard();
}
void LevelManager::HandleMouse(RAWINPUT MouseData){
	currentLevel->HandleMouse(MouseData);
}
void LevelManager::Render(LPDIRECT3DDEVICE9& d3ddev) { currentLevel->Render(d3ddev);}


