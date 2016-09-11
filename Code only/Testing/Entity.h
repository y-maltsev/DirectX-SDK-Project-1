#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "Globalincludes.h"
#include "CModel.h"

struct Entity{
CModel* Mesh; //the model
D3DXVECTOR3 Position; // position in the world
int health;
};


#endif