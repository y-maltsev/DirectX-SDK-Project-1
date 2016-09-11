#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__
#include "GlobalIncludes.h"
class UserInterface{
	LPD3DXSPRITE d3dspt;
	LPDIRECT3DTEXTURE9 life,health,target;
	LPD3DXFONT dxfont; 

public:
    UserInterface(LPDIRECT3DDEVICE9& d3ddev);
	void Draw();
};

#endif