#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#include "Globalincludes.h"
#include "Entity.h"
#define TERRAIN_X			255
#define TERRAIN_Z			255


class Terrain
{
	class enviroment_obj{
		LPD3DXMESH Mesh;
		D3DMATERIAL9* materials;
		DWORD numMaterials;
	public:
		enviroment_obj(char* file_name,LPDIRECT3DDEVICE9& d3ddev){
		
			LPD3DXBUFFER bufferMaterials;
			D3DXLoadMeshFromX(file_name,D3DXMESH_SYSTEMMEM,d3ddev, NULL,&bufferMaterials,NULL,&numMaterials,&Mesh);
			D3DXMATERIAL* tempMaterials=(D3DXMATERIAL*)bufferMaterials->GetBufferPointer();
			materials=new D3DMATERIAL9[numMaterials];
			for(DWORD i=0;i<numMaterials;i++){
				materials[i]=tempMaterials[i].MatD3D;
				materials[i].Ambient=materials[i].Diffuse;
			}
		}
		void Draw(LPDIRECT3DDEVICE9& d3ddev){
			for(DWORD i=0;i<numMaterials;i++){
				d3ddev->SetMaterial(&materials[i]);
				Mesh->DrawSubset(i);
			}
		}
	};
	LPDIRECT3DVERTEXBUFFER9	m_pTerrainVB; //the vertex container
	LPDIRECT3DINDEXBUFFER9	m_pTerrainIB; //the index container(to make the faces)

	DWORD					m_dwTerrainVertices, //the total number of vertices
							m_dwTerrainPrimitives; //the total number of primitives(faces/poligons)
	LPDIRECT3DTEXTURE9 texture; //the texture of the terrain
	void LoadHeighMap(char* hightmap);
	void LoadVertexBuffer(LPDIRECT3DDEVICE9& d3ddev);
	void LoadIndexBuffer(LPDIRECT3DDEVICE9& d3ddev);
	enviroment_obj* Grass;
	D3DXVECTOR3 PositionGrass[40];
	enviroment_obj* Trees;
	D3DXVECTOR3 PositionTrees[40];
public:
	
	unsigned char** HeightMap; //the heightmap(the height for each vertex on the terrain) container

	Terrain(LPDIRECT3DDEVICE9& d3ddev);
	void CreateTerrain(LPDIRECT3DDEVICE9& d3ddev);
	void Draw(LPDIRECT3DDEVICE9& d3ddev);

	float Get_height(float x,float z); //get the height(calculated from heightmap) for x,z cordinates

};
#endif