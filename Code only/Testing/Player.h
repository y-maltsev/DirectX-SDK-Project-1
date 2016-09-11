#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Globalincludes.h"
#include "Entity.h"


struct Player:Entity {
	static int lifes;
	bool moving_left,moving_right,moving_forward,moving_backwards;
	float Rotate;
	int Score;
	float jump_fall_speed;
	DWORD Reverse_time;
	Player(LPDIRECT3DDEVICE9& d3ddev);
	void Update(D3DXVECTOR3 C_Position,D3DXVECTOR3 C_LookAtPos);
	void Move_animation();
	void Move(D3DXVECTOR3 C_Position,D3DXVECTOR3 C_LookAtPos);
	void Draw(LPDIRECT3DDEVICE9& d3ddev);
	~Player();
};

#endif