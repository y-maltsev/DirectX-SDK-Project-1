#include "Game.h"

Game game;
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
game.init(hInstance,nCmdShow);
game.run();
game.end();
}