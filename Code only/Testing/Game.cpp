#include "Game.h"
#define KEY_DOWN(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetKeyState(vk_code) & 0x8000) ? 0 : 1)

Camera *Game::camera;
Terrain *Game::terrain;
Player* Game::player;
vector<Enemy*> Game::enemyes;
Skybox* Game::skybox;
UserInterface* Game::UI;
LPDIRECT3DDEVICE9 Game::d3ddev;

Game::Game()
	:FPS(20),
	 TIME_PER_FRAME(1000 / FPS),
	 FRACTION_TIME_PER_FRAME(TIME_PER_FRAME / 1000.0f){}

void Game::init(HINSTANCE hInstance, int nCmdShow){
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Game::WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = (LPCSTR)"WindowClass";


    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, (LPCSTR)"WindowClass", (LPCSTR)"First person shooter",
                           WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    initD3D(hWnd);
	init_input(hWnd);

	camera = new Camera();
	terrain = new Terrain(d3ddev);
	//player = new Player(d3ddev);
	//camera->Attach(player);
	skybox = new Skybox(d3ddev);
	UI = new UserInterface(d3ddev);


	levelmanager= new LevelManager(); 
	GameLevel* mainmenu = new MainMenu();
	mainmenu->Init();
	LevelManager::currentLevel = mainmenu;
}
void Game::initD3D(HWND& hWnd)
{
    LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
	
    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);    // mipmap
	//d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // turn off culling
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50)); //set global light

	D3DLIGHT9 light;    // create the light struct
    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(-1.0f, -1.5f,-1.0f); 
    d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    d3ddev->LightEnable(0, TRUE);    // turn on light #0

	ShowCursor(false);
}
void Game::init_input(HWND& hWnd)
{
    RAWINPUTDEVICE Mouse;
    Mouse.usUsage = 0x02;    // register mouse
    Mouse.usUsagePage = 0x01;    // top-level mouse
    Mouse.dwFlags = NULL;    // flags
    Mouse.hwndTarget = hWnd;    // handle to a window

    RegisterRawInputDevices(&Mouse, 1, sizeof(RAWINPUTDEVICE));    // register the device
}
void Game::run(){
	DWORD lastTime= GetTickCount();
	MSG msg;
	srand ( time(NULL) );
    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;
		DWORD currentTime = GetTickCount();
		while( currentTime - lastTime >= TIME_PER_FRAME)
        {
            update();
            lastTime += TIME_PER_FRAME;
        }
		render();	
    }
}

Game::Ray Game::Picking_CalcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;
	D3DVIEWPORT9 vp;
	d3ddev->GetViewport(&vp);
	D3DXMATRIX proj;
	d3ddev->GetTransform(D3DTS_PROJECTION, &proj);
	px = ((( 2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);
	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);
	return ray;
}

void Game::Picking_TransformRay(Ray* ray, D3DXMATRIX* T)
	{
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(&ray->_origin,&ray->_origin, T);
	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(&ray->_direction,&ray->_direction,T);
	// normalize the direction
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

bool Game::Picking_raySphereIntersection(Ray* ray,BoundingSphere* sphere)
{
	D3DXVECTOR3 v = ray->_origin - sphere->_center;
	float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);
	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);
	// test for imaginary number
	if( discriminant < 0.0f )
	return false;
	discriminant = sqrtf(discriminant);
	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;
	// if a solution is >= 0, then we intersected the sphere
	if( s0 >= 0.0f || s1 >= 0.0f )
	return true;
	return false;
}

void Game::Picking(){
		// compute the ray in view space given the clicked screen point
	Ray ray = Picking_CalcPickingRay(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	// transform the ray to world space
	D3DXMATRIX view;
	d3ddev->GetTransform(D3DTS_VIEW, &view);
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);
	Picking_TransformRay(&ray, &viewInverse);
	// test for a hit
	BoundingSphere BSphere;
	BSphere._radius=1.0f;
	for(unsigned int i=0;i<enemyes.size();i++){
		BSphere._center.x=enemyes[i]->Position.x;
		BSphere._center.y=enemyes[i]->Position.y+0.5f;
		BSphere._center.z=enemyes[i]->Position.z;
		if( Picking_raySphereIntersection(&ray, &BSphere) )
		{		
			if(player!=NULL){
				enemyes[i]->target=player;
				enemyes[i]->health--;
			}
			else
				camera->Attach(Game::enemyes[i]);
		}
	}
}


void Game::HandleMouse(RAWINPUT InputData){

	GetCursorPos(&MousePos);    // get the mouse data
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);    // move the mouse to the center
	MousePos.x -= SCREEN_WIDTH / 2;
	MousePos.y -= SCREEN_HEIGHT / 2;
	camera->RotateY += MousePos.x * 0.002f;
	camera->RotateX -= MousePos.y * 0.002f;
	
	static BOOL MouseDown;
	if(InputData.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN){
			SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			MouseDown = TRUE;
			Picking();
	}
	if(InputData.data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN){
			SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}
	
	if(InputData.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP){
        MouseDown = FALSE;
	}
	if(MouseDown){
		//not used yet
	}	
}

void Game::HandleKeyboard()
{
	D3DXVECTOR3 Direction;
		if( KEY_DOWN(0x57))//keyboard key "w"
		{
			if(camera->attached && player!=NULL){
			
				player->moving_forward=true;	
			}
			else camera->Move(camera->C_LookAtPos - camera->C_Position);	
		}
		if( KEY_DOWN(0x53))//keyboard key "s"
		{
			if(camera->attached && player!=NULL)player->moving_backwards=true;
			else camera->Move(camera->C_Position - camera->C_LookAtPos);
		}
		if( KEY_DOWN(0x41))//keyboard key "a"
		{
			
			if(camera->attached && player!=NULL)
			{
			
				player->moving_left=true;
			}
			else 
			{
				D3DXVec3Cross(&Direction,&(camera->C_LookAtPos - camera->C_Position),&camera->C_UpVect);	
				camera->Move(Direction);
			}
		
		}
		if( KEY_DOWN(0x44))//keyboard key "d"
		{
			
			if(camera->attached && player!=NULL){
			
				player->moving_right=true;
			}
			else 
			{
				D3DXVec3Cross(&Direction,&(camera->C_Position- camera->C_LookAtPos ),&camera->C_UpVect);
				camera->Move(Direction);
			}
		}
		if(camera->attached && player!=NULL){
			if(KEY_UP(0x57))//keyboard key "w"
			{
				player->moving_forward=false;
			}
			if(KEY_UP(0x53))//keyboard key "s"
			{
				player->moving_backwards=false;
			}
			if(KEY_UP(0x41))//keyboard key "a"
			{
				player->moving_left=false;
				
			}
			if(KEY_UP(0x44))//keyboard key "d"
			{
				player->moving_right=false;
			}
		}	
		if(KEY_DOWN(0x20)){//keyboard key "space"
			if(player!=NULL){
				if(Game::terrain->Get_height(player->Position.x,player->Position.z)+0.8f==player->Position.y)
					player->jump_fall_speed=0.5f;
			}
			else if(!camera->attached){
				camera->C_LookAtPos.y+=1;
				camera->C_Position.y+=1;	
			}
		}
		
		if(KEY_DOWN(0x45)){//keyboard key "e"
			if(player!=NULL) delete player;
			player = new Player(d3ddev);
			camera->Attach(player);
			Player::lifes--;
		}

		if(KEY_DOWN(0x31)){//keyboard key "1"
			d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME );
		}
		if(KEY_DOWN(0x32)){//keyboard key "2"
			d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID );
		}
		if(KEY_DOWN(0x33)){//keyboard key "3"
			 ShowCursor( FALSE );
		}
		if(KEY_DOWN(0x34)){ //keyboard key "4"
			 ShowCursor( TRUE );
		}
		
		if(KEY_DOWN(0x39)){ //keyboard key "9"
			for(unsigned int i=0;i<enemyes.size();i++)
				enemyes[i]->health-=9;
		}	
		if(KEY_DOWN(0x1B)){//keyboard key "Escape"
			MainMenu* mainmenu = new MainMenu();
			mainmenu->Init();
			mainmenu->savelevel=LevelManager::currentLevel;
			LevelManager::currentLevel = mainmenu;
		}
}



LRESULT CALLBACK Game::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} break;
		case WM_INPUT:
		{
			RAWINPUT InputData;

			UINT DataSize = sizeof(RAWINPUT);
			GetRawInputData((HRAWINPUT)lParam,
							RID_INPUT,
							&InputData,
							&DataSize,
							sizeof(RAWINPUTHEADER));
			extern Game game;
			game.levelmanager->HandleMouse(InputData);
			return 0;
		} break;
		case WM_KEYDOWN:
		{
			extern Game game;
			game.levelmanager->HandleKeyboard();
		}break;
		case WM_KEYUP:
		{
			extern Game game;
			game.levelmanager->HandleKeyboard();
		}break;

    }
    return DefWindowProc (hWnd, message, wParam, lParam);
}

void Game::render(){

    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();

    d3ddev->SetFVF(CUSTOMFVF);
	// set the world transform
	
    // set the projection transform
    D3DXMATRIX matProjection;    // the projection transform matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,   // the near view-plane
                               5000.0f);    // the far view-plane
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection
	levelmanager->Render(d3ddev);	
    d3ddev->EndScene(); 

    d3ddev->Present(NULL, NULL, NULL, NULL);


}
void Game::update(){
	levelmanager->Update();
}

void Game::end(){
	delete terrain;
	delete camera;
    d3ddev->Release();
}