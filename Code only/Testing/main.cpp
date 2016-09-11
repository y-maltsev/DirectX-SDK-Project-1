// include the basic windows header files and the Direct3D header file
/*
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include<iostream>
#include "Camera.h"
#include "Terrain.h"
using namespace std;
// define the screen resolution
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1000
// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
POINT MousePos;

// function prototypes
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_input(HWND hWnd);
void init_light(void);


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



//------------------------------------------------------------------------------------------------------------------------
//Camera
//------------------------------------------------------------------------------------------------------------------------
/*
class Camera{
	D3DXVECTOR3 C_Postion,
				C_LookAtPos,
				C_UpVect; //the camera "up" direction
	
	public:
		float SensitivityX;
		float SensitivityY;
			Camera();
			void Move();
			void Rotate();
			void Update();
};
Camera::Camera(){
C_Postion = D3DXVECTOR3(-50.0f,-50.0f,50.0f);
C_LookAtPos = D3DXVECTOR3(50.0f,50.0f,0.0f);
C_UpVect = D3DXVECTOR3(0.0f,0.0f,1.0f);
SensitivityX=0.0f;
SensitivityY=0.0f;
}
void Camera::Update(){
	Move();
	Rotate();
	D3DXMATRIX	m_matView;
	D3DXMatrixLookAtLH(&m_matView,&C_Postion,&C_LookAtPos,&C_UpVect);
	d3ddev->SetTransform(D3DTS_VIEW,&m_matView);
}
void Camera::Move(void){
	D3DXVECTOR3 Direction;

	D3DXVec3Normalize(&Direction,&(C_LookAtPos - C_Postion));

	if(KEY_DOWN(0x57))
	{
	  C_Postion += Direction * 0.5f;
	  C_LookAtPos += Direction * 0.5f;
	}

	if(KEY_DOWN(0x53))
	{
	  C_Postion -= Direction * 0.5f;
	  C_LookAtPos -= Direction * 0.5f;
	}

	if(KEY_DOWN(0x41))
	{
	 D3DXVec3Cross(&Direction,&Direction,&C_UpVect);
	 D3DXVec3Normalize(&Direction,&Direction);
	 C_Postion += Direction * 0.5f;
	 C_LookAtPos += Direction * 0.5f;
	}

	if(KEY_DOWN(0x44))
	{
	 D3DXVec3Cross(&Direction,&Direction,&C_UpVect);
	 D3DXVec3Normalize(&Direction,&Direction);
	 C_Postion -= Direction * 0.5f;
	 C_LookAtPos -= Direction * 0.5f;
	}
	if(KEY_DOWN(VK_SPACE))
	{
	 D3DXVec3Normalize(&Direction,&C_UpVect);
	 C_Postion += Direction * 0.5f;
	 C_LookAtPos += Direction * 0.5f;
	}
	if(KEY_DOWN(VK_CONTROL))
	{
	 D3DXVec3Normalize(&Direction,&C_UpVect);
	 C_Postion -= Direction * 0.5f;
	 C_LookAtPos -= Direction * 0.5f;
	}
	//view types
	if(KEY_DOWN(0x31))
	{
	d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}
	if(KEY_DOWN(0x32))
	{
	d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	}
	if(KEY_DOWN(0x33))
	{
	d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);
	}
}
void Camera::Rotate(){
	D3DXVECTOR3 Direction,RotAxis;
	D3DXMATRIX matRotAxis,matRotZ;

	D3DXVec3Normalize(&Direction,&(C_LookAtPos - C_Postion));
	D3DXVec3Cross(&RotAxis,&Direction,&C_UpVect);
	D3DXVec3Normalize(&RotAxis,&RotAxis);

	D3DXMatrixRotationAxis(&matRotAxis,&RotAxis,SensitivityX);
	D3DXMatrixRotationZ(&matRotZ,SensitivityY);

	D3DXVec3TransformCoord(&Direction,&Direction,&(matRotAxis * matRotZ));
	D3DXVec3TransformCoord(&C_UpVect,&C_UpVect,&(matRotAxis * matRotZ));
	C_LookAtPos = Direction + C_Postion;
}

//------------------------------------------------------------------------------------------------------------------------
//Terrain
//------------------------------------------------------------------------------------------------------------------------
/*
#define TERRAIN_X			255
#define TERRAIN_Y			255
class CTerrain
{
	
	LPDIRECT3DVERTEXBUFFER9	m_pTerrainVB;
	LPDIRECT3DINDEXBUFFER9	m_pTerrainIB;

	DWORD					m_dwTerrainVertices,
							m_dwTerrainPrimitives;
public:
							CTerrain(void);

	void					CreateTerrain(void);

	void					Draw(void);


};

CTerrain::CTerrain(void)
{
m_pTerrainVB = NULL;
m_pTerrainIB = NULL;

m_dwTerrainVertices = (TERRAIN_X + 1) * (TERRAIN_Y + 1);
m_dwTerrainPrimitives = TERRAIN_X * TERRAIN_Y * 2;

CreateTerrain();
}//CTerrain


void CTerrain::CreateTerrain(void)
{
unsigned char aHeightMap[TERRAIN_X + 1][TERRAIN_Y + 1];
CUSTOMVERTEX* pVertexData;
short* pIndexData=0;
ifstream File;

//load height information from file
File.open("heightmap.raw",ios::binary);
 
for(unsigned y = 0;y < (TERRAIN_Y + 1);++y)
 {
 for(unsigned x = 0;x < (TERRAIN_X + 1);++x)
  {
  aHeightMap[x][y] = File.get();
  }
 }

File.close();

//create vertex buffer and fill in vertcies
d3ddev->CreateVertexBuffer(m_dwTerrainVertices * sizeof(CUSTOMVERTEX),0,CUSTOMFVF,D3DPOOL_MANAGED,&m_pTerrainVB,NULL);
m_pTerrainVB->Lock(0,0,(void**)&pVertexData,0);

for(int y = 0;y < (TERRAIN_Y + 1);++y)
 {
 for(int x = 0;x < (TERRAIN_X + 1);++x)
  { 
  pVertexData[x + y * (TERRAIN_X + 1)].X = (float)x;
  pVertexData[x + y * (TERRAIN_X + 1)].Y = (float)y;
  pVertexData[x + y * (TERRAIN_X + 1)].Z =-(float)aHeightMap[x][y]/2;
  }
 }

for(int y = 0;y < (TERRAIN_Y + 1);++y)
 {
 for(int x = 0;x < (TERRAIN_X + 1);++x)
  { 
	float heightA = (float)aHeightMap[x][y]/2;
	float heightB = (float)aHeightMap[x][y+1]/2;
	float heightC = (float)aHeightMap[x+1][y]/2;
	D3DXVECTOR3 u(1.0f, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, -1.0f);
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);
	pVertexData[x + y * (TERRAIN_X + 1)].NORMAL = n;

 }
}
  
 m_pTerrainVB->Unlock();

//create index buffer and fill in indices
d3ddev->CreateIndexBuffer(sizeof(short) * m_dwTerrainPrimitives * 3,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pTerrainIB,NULL);
m_pTerrainIB->Lock(0,0,(void**)&pIndexData,0);

for(unsigned y = 0;y < TERRAIN_Y;++y)
 {
 for(unsigned x = 0;x < TERRAIN_X;++x)
  {
  *pIndexData++ = x + y * (TERRAIN_X + 1);
  *pIndexData++ = x + 1 + y * (TERRAIN_X + 1);
  *pIndexData++ = x + 1 + (y + 1) * (TERRAIN_X + 1);

  *pIndexData++ = x + y * (TERRAIN_X + 1);
  *pIndexData++ = x + 1 + (y + 1) * (TERRAIN_X + 1);
  *pIndexData++ = x + (y + 1) * (TERRAIN_X + 1);
  }
 }

m_pTerrainIB->Unlock();
}//CreateTerrain



void CTerrain::Draw(void)
{
d3ddev->SetStreamSource(0,m_pTerrainVB,0,sizeof(CUSTOMVERTEX));
d3ddev->SetIndices(m_pTerrainIB);
d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_dwTerrainVertices,0,m_dwTerrainPrimitives);
}//Draw 
CTerrain* pTerrain;
Camera* pCamera;
// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));ww

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = (LPCSTR)L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, (LPCSTR)L"WindowClass", (LPCSTR)L"Our Direct3D Program",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    initD3D(hWnd);
	init_input(hWnd);

    MSG msg;
    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

        render_frame();
    }

    cleanD3D();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			
			static BOOL MouseDown;
			if(InputData.data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN){
					SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                    MouseDown = TRUE;
			}
                if(InputData.data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)
                    MouseDown = FALSE;

                // rotate the teapot
				if(MouseDown){
					GetCursorPos(&MousePos);    // get the mouse data
					SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);    // move the mouse to the center
					MousePos.x -= SCREEN_WIDTH / 2;
					MousePos.y -= SCREEN_HEIGHT / 2;
					pCamera->SensitivityY += MousePos.x * 0.01f;
					pCamera->SensitivityX -= MousePos.y * 0.01f;
				}
		

			return 0;
		} break;

    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

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

    init_graphics();
	init_light();
	
    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
    d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // turn off culling
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(200, 200, 200)); //set global light
}


// this is the function used to render a single frame
void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();

    d3ddev->SetFVF(CUSTOMFVF);
	    // set the world transform
	pCamera->Update(d3ddev);
	pCamera->SensitivityX=0.0f;
	pCamera->SensitivityY=0.0f;
    // set the projection transform
    D3DXMATRIX matProjection;    // the projection transform matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,   // the near view-plane
                               500.0f);    // the far view-plane
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

	pTerrain->Draw(d3ddev);
    d3ddev->EndScene(); 

    d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	delete pTerrain;
	delete pCamera;
    d3ddev->Release();
    d3d->Release();
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	pCamera = new Camera(d3ddev);
	pTerrain = new CTerrain( d3ddev);
}
void init_input(HWND hWnd)
{
    RAWINPUTDEVICE Mouse;
    Mouse.usUsage = 0x02;    // register mouse
    Mouse.usUsagePage = 0x01;    // top-level mouse
    Mouse.dwFlags = NULL;    // flags
    Mouse.hwndTarget = hWnd;    // handle to a window

    RegisterRawInputDevices(&Mouse, 1, sizeof(RAWINPUTDEVICE));    // register the device
}
// this is the function that sets up the lights and materials
void init_light(void)
{
    D3DLIGHT9 light;    // create the light struct
    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(-1.0f, -1.5f,-1.0f); 

    d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    d3ddev->LightEnable(0, TRUE);    // turn on light #0

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.0f, 1.0f);    // set diffuse color to yellow
    material.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.0f, 1.0f);    // set ambient color to yellow

    d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}*/