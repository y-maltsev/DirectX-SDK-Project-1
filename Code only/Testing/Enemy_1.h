#ifndef __ENEMY_1_H__
#define __ENEMY_1_H__
#include "Enemyes.h"

struct Enemy_1: Enemy {

	 D3DXVECTOR3 target_pos; // if there is target(Entity*) target_pos equals target's position. But if there isnt target(Entity*) its just point(X,Y,Z) at the terrain
	 float Rotation; //rotate the model so it can look at the same direction as it moves
	 Enemy_1(LPDIRECT3DDEVICE9& d3ddev, D3DXVECTOR3 Starting_pos);
	 void Update();
	 void Move();
	 void Draw(LPDIRECT3DDEVICE9& d3ddev);
	 ~Enemy_1();
};

#endif