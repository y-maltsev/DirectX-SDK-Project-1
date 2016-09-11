#ifndef __GAME_H__
#define __GAME_H__
#include "Globalincludes.h"
#include "Camera.h"
#include "Terrain.h"
#include "LevelManager.h"
#include "level_1.h"
#include "Player.h"
#include "Enemyes.h"
#include "Enemy_1.h"
#include "Skybox.h"
#include "UserInterface.h"
#include "MainMenu.h"
using namespace std;

class Game{

POINT MousePos;
LevelManager* levelmanager; 

void initD3D(HWND& hWnd);
void init_input(HWND& hWnd);

	struct Ray
	{
		D3DXVECTOR3 _origin;
		D3DXVECTOR3 _direction;
	};

	struct BoundingSphere
	{
		D3DXVECTOR3 _center;
		float _radius;
	};
	Ray Picking_CalcPickingRay(int x, int y);
	void Picking_TransformRay(Ray* ray, D3DXMATRIX* T);
	bool Picking_raySphereIntersection(Ray* ray,BoundingSphere* sphere);
	void Picking();

void render();
void update();

//Frames Per Second variables
const int FPS;
const DWORD TIME_PER_FRAME;
const float FRACTION_TIME_PER_FRAME;

public:

	Game();
	void init(HINSTANCE hInstance, int nCmdShow);
	void run(); 
	void end();

	void HandleKeyboard();
	void HandleMouse(RAWINPUT InputData);


	static LPDIRECT3DDEVICE9 d3ddev;
	static Terrain* terrain;
	static Camera* camera;
	static Player* player;
	static vector<Enemy*> enemyes;
	static Skybox* skybox;
	static UserInterface* UI;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


};


#endif