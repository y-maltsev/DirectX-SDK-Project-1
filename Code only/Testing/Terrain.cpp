#include "Terrain.h"



Terrain::Terrain(LPDIRECT3DDEVICE9& d3ddev)
{
	m_pTerrainVB = NULL;//clear the vertices buffer(container)
	m_pTerrainIB = NULL;//clear the indices buffer(container)
	HeightMap=NULL; //make sure its clear
	//set the number of vertices and primitives(faces/poligons)
	m_dwTerrainVertices = (TERRAIN_X + 1) * (TERRAIN_Z + 1);
	m_dwTerrainPrimitives = TERRAIN_X * TERRAIN_Z * 2;
	//load the information for the vertices and indices
	CreateTerrain(d3ddev);

	//load the texture
	texture=NULL;//make sure its clear
	D3DXCreateTextureFromFile(d3ddev,    // the Direct3D device
                              (LPCSTR)"Media/Grass_2.png",    // the filename of the texture
                              &texture);    // the address of the texture storage
	if(texture==NULL) //if the texture's load have failed
		 ::MessageBoxW(NULL, L"Cant load Texture", L"Failed", MB_OK);
	Trees = new enviroment_obj("Media/palm.x",d3ddev);
	Grass = new enviroment_obj("Media/grass.x",d3ddev);
	for(int i=0;i<40;++i){
		PositionTrees[i].x=rand()%254+1;
		PositionTrees[i].z=rand()%254+1;
		PositionTrees[i].y=Get_height(PositionTrees[i].x,PositionTrees[i].z);
	}
	for(int i=0;i<40;++i){
		PositionGrass[i].x=rand()%254+1;
		PositionGrass[i].z=rand()%254+1;
		PositionGrass[i].y=Get_height(PositionGrass[i].x,PositionGrass[i].z);
	}
}


void Terrain::LoadHeighMap(char* hightmap){

ifstream File;

//open the file
File.open(hightmap,ios::binary);
//set the size of the heightmap
HeightMap = new unsigned char*[TERRAIN_X + 1];
//load height information from file
for(unsigned x = 0;x < (TERRAIN_X + 1);++x)
 {
	HeightMap[x] = new unsigned char[TERRAIN_X + 1];

 for(unsigned z = 0;z < (TERRAIN_Z + 1);++z)
  {
  HeightMap[x][z] = File.get();
  }
 }

File.close();
}

void Terrain::LoadVertexBuffer(LPDIRECT3DDEVICE9& d3ddev){
CUSTOMVERTEX* pVertexData;

//create vertex buffer and fill in vertcies
d3ddev->CreateVertexBuffer(m_dwTerrainVertices * sizeof(CUSTOMVERTEX),0,CUSTOMFVF,D3DPOOL_MANAGED,&m_pTerrainVB,NULL);
m_pTerrainVB->Lock(0,0,(void**)&pVertexData,0);

for(int x = 0;x < (TERRAIN_X + 1);++x)
  { 
	for(int z = 0;z < (TERRAIN_Z + 1);++z)
	{
		pVertexData[x + z * (TERRAIN_X + 1)].X = (float)x;
		pVertexData[x + z * (TERRAIN_X + 1)].Y = (float)HeightMap[x][z]/6;
		pVertexData[x + z * (TERRAIN_X + 1)].Z = (float)z;
		pVertexData[x + z * (TERRAIN_X + 1)].U = (float)((int)z%2);
		pVertexData[x + z * (TERRAIN_X + 1)].V = (float)((int)x%2);
	}
 }

//set shading information for the terrain
for(int x = 0;x < (TERRAIN_X);++x)
{ 
	for(int z = 0;z < (TERRAIN_Z);++z)
	{
		float heightA = (float)HeightMap[x+1][z]/6;
		float heightB = (float)HeightMap[x][z]/6;
		float heightC = (float)HeightMap[x][z+1]/6;
		D3DXVECTOR3 u(1.0f, heightB - heightA, 0.0f);
		D3DXVECTOR3 v(0.0f, heightC - heightA, -1.0f);
		D3DXVECTOR3 n;
		D3DXVec3Cross(&n, &u, &v);
		D3DXVec3Normalize(&n, &n);
		n.x=-n.x;
		n.z=-n.z;
		pVertexData[x + z * (TERRAIN_X + 1)].NORMAL = n;
	}
}
  
 m_pTerrainVB->Unlock();

}

void Terrain::LoadIndexBuffer(LPDIRECT3DDEVICE9& d3ddev){

short* pIndexData=0;
//create index buffer and fill in indices
d3ddev->CreateIndexBuffer(sizeof(short) * m_dwTerrainPrimitives * 3,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pTerrainIB,NULL);
m_pTerrainIB->Lock(0,0,(void**)&pIndexData,0);


for(unsigned x = 0;x < TERRAIN_X;++x)
{
	for(unsigned z = 0;z < TERRAIN_Z;++z)
	{
	  *pIndexData++ = z + x * (TERRAIN_X + 1);
	  *pIndexData++ = z + 1 + (x + 1) * (TERRAIN_X + 1);
	  *pIndexData++ = z + 1 + x * (TERRAIN_X + 1);
	  

	  *pIndexData++ = z + x * (TERRAIN_X + 1);
	  
	  *pIndexData++ = z + (x + 1) * (TERRAIN_X + 1);
	  *pIndexData++ = z + 1 + (x + 1) * (TERRAIN_X + 1);
	}
}
m_pTerrainIB->Unlock();
}


void Terrain::CreateTerrain(LPDIRECT3DDEVICE9& d3ddev)
{
	LoadHeighMap("Media/heightmap.raw");
	LoadVertexBuffer(d3ddev);
	LoadIndexBuffer(d3ddev);
}


//CreateTerrain

/*----------------------------------------------------------------------*/
/* Name:		Draw													*/
/* Description:	draw terrain											*/
/*----------------------------------------------------------------------*/

void Terrain::Draw(LPDIRECT3DDEVICE9& d3ddev)
{
	D3DXMATRIX matTranslation,matRotX,matScale;
	D3DXMatrixTranslation(&matTranslation, 0.0f,0.0f,0.0f);   
	d3ddev->SetTransform(D3DTS_WORLD,&matTranslation );
	d3ddev->SetFVF(CUSTOMFVF);

	D3DMATERIAL9 material;    // create the material struct
	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
	d3ddev->SetMaterial(&material);    // set the globably-used material to &material
	d3ddev->SetStreamSource(0,m_pTerrainVB,0,sizeof(CUSTOMVERTEX));
	d3ddev->SetIndices(m_pTerrainIB);
	d3ddev->SetTexture(0, texture);
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_dwTerrainVertices,0,m_dwTerrainPrimitives);
	for(int i=0;i<20;i++){
		D3DXMatrixRotationX(&matRotX,4.71);// the trees' models normaly lie down so we need to turn them 90 degree to normal
		D3DXMatrixTranslation(&matTranslation, PositionTrees[i].x,PositionTrees[i].y,PositionTrees[i].z);   //set the position
		d3ddev->SetTransform(D3DTS_WORLD, &(matRotX *matTranslation));
		Trees->Draw(d3ddev); //1/2 Main reasons for the slow execution of the game, no "lag" without them
	}
	for(int i=0;i<40;i++){
		D3DXMatrixRotationX(&matRotX,4.71);// the grass's model normaly lies down so we need to turn it 90 degree to normal
		D3DXMatrixTranslation(&matTranslation, PositionGrass[i].x,PositionGrass[i].y,PositionGrass[i].z);   //set the position
		D3DXMatrixScaling(&matScale, 0.002f, 0.002f, 0.002f); //and reduse it size to normal
		d3ddev->SetTransform(D3DTS_WORLD, &(matRotX *matScale * matTranslation));
		//Grass->Draw(d3ddev); //2/2 Main reasons for the slow execution of the game
	}
}

float Terrain::Get_height(float x,float z){
	if(x<0 || x>255 || z<0 || z>255) return 0;

	float height;
	float A = HeightMap[(int)x][(int)z];
	float B = HeightMap[(int)x+1][(int)z];
	float C = HeightMap[(int)x][(int)z+1];
	float D = HeightMap[(int)x+1][(int)z+1];
	float dx = x - (int)x;
	float dz = z - (int)z;

	if(dz < 1.0f - dx) // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		height = A + uy*dx + vy*dz;
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
		height = D + (uy*(1.0f - dx))+(vy*(1.0f - dz));
	} return height/6;
}



