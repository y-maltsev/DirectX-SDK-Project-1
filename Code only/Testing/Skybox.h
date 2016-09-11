#ifndef __SKYBOX_H__
#define __SKYBOX_H__
#include "Globalincludes.h"
class Skybox {

	LPD3DXMESH mesh;
	DWORD numMaterials;
	D3DMATERIAL9 *meshMaterials;
	LPDIRECT3DTEXTURE9 *meshTextures;
	LPD3DXBUFFER materialBuf;
public:
	Skybox(LPDIRECT3DDEVICE9& device);
	void Draw(LPDIRECT3DDEVICE9& device);
};
#endif