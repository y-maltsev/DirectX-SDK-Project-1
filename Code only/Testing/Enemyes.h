#ifndef __ENEMYES_H__
#define __ENEMYES_H__
#include "Entity.h"


struct Enemy: Entity {
Entity* target;
int ScorePoints;
float Rotate; //save value to rotate the model in the same direction as the movement direction
virtual void Update()=0;
virtual void Move()=0;
virtual void Draw(LPDIRECT3DDEVICE9& d3ddev)=0;
virtual ~Enemy();

static void Set_Height(Enemy* enemy); //set the hight of "Enemy* enemy" at the terrain


};


#endif