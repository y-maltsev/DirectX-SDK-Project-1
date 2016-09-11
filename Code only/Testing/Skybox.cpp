#include "Globalincludes.h"
#include "Skybox.h"


Skybox::Skybox(LPDIRECT3DDEVICE9& device){

HRESULT hr = D3DXLoadMeshFromX("Media/skybox.x", D3DXMESH_SYSTEMMEM, device, NULL, &materialBuf, NULL, &numMaterials, &mesh);
	
	//create the containers for materials and textures
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)materialBuf->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numMaterials];
	meshTextures  = new LPDIRECT3DTEXTURE9[numMaterials];
	
	//copy the information for materials and textures from the .x file to the skybox ingame object
	for (DWORD i=0; i<numMaterials; i++)
	{
		 meshMaterials[i] = d3dxMaterials[i].MatD3D;
		 meshMaterials[i].Ambient = meshMaterials[i].Diffuse; //make the Ambitient and Diffuse shading the same
		 meshTextures[i] = NULL;
		 if (d3dxMaterials[i].pTextureFilename)
		 {	
			D3DXCreateTextureFromFile(device, d3dxMaterials[i].pTextureFilename, &meshTextures[i]);
		 }
			
	}

}

void Skybox::Draw(LPDIRECT3DDEVICE9& device){
	for(int i=0;i<numMaterials;i++){//go through all subsets
		D3DXMATRIX matScale;    
		D3DXMatrixScaling(&matScale, 200.0f, 200.0f, 200.0f); //increase the size of the skybox so much that it would look like sky
		device->SetTransform(D3DTS_WORLD, &matScale);
		device->SetMaterial(&meshMaterials[i]);    // set the material for the subset
		device->SetTexture(0, meshTextures[i]);    // set the texture for the subset
        mesh->DrawSubset(i);    // draw the subset
	}
}