#define _USE_MATH_DEFINES
#include "Globalincludes.h"
#include "Enemy_1.h"
#include <math.h>
#include "Game.h"

Enemy_1::Enemy_1(LPDIRECT3DDEVICE9& d3ddev, D3DXVECTOR3 Starting_pos){
	// Create the model
	Mesh = new CModel(d3ddev);
	//Load the new model
	Mesh->LoadXFile("Media/tiny.x");
	Mesh->SetCurrentAnimation(0);
	Position = Starting_pos;
	Rotate=0.0f;
	health = 10;
	target = NULL;
	target_pos=Position;
	ScorePoints=100;
}

void Enemy_1::Update(){
	if(health<=0) { //check is the enemy dead
		if(Game::player!=NULL) //if the player is alive
		Game::player->Score+=ScorePoints; //increase his score
		delete this; // then delete the enemy
		return;
	}
	if(target!=NULL) { //does the enemy have target(object)
		target_pos = target->Position;
		if(Position.x < target_pos.x+2 && Position.x > target_pos.x-2 && //check if the target(object) is within range - 2 
		   Position.z < target_pos.z+2 && Position.z > target_pos.z-2   ) target->health--; //if so forbid movement/animation and decrease the target's health
		else {
			Mesh->Update(); //If no target is in range, continue the animation and movement
			Move();
		}
	}
	else //if the enemy doesnt have target(object) - imitate free movement
	{
		if(Position.x < target_pos.x+1 && Position.x > target_pos.x-1 &&
			Position.z < target_pos.z+1 && Position.z > target_pos.z-1){ //if the enemy has reached its taget(X,Z cordinates)
			target_pos.x=rand()%254+1;  //make new target to move to on the field(X,Z cordinates)
 			target_pos.z=rand()%254+1;		
		 }
		Mesh->Update();
		Move();
	}
	Enemy::Set_Height(this);
	Position.y+=0.9f; //increase the height more because of the model
}

void Enemy_1::Move(){ //homing movement
		D3DXVECTOR3 movement=target_pos-Position;
		movement.y=0;
		D3DXVec3Normalize(&movement,&movement);
		Position+=movement*0.1;
		if(movement.x>0) //rotation
			if(!movement.z>0)Rotate=-asin(movement.z);
			else Rotate=asin(movement.z);
		else 
			if(!movement.z>0) Rotate=asin(movement.z)-110;
			else Rotate=-asin(movement.z) -110 ;
}

void Enemy_1::Draw(LPDIRECT3DDEVICE9 &d3ddev){
	D3DXMATRIX  matScale,matTranslation,matRotY,matRotX;
	D3DXMatrixRotationY(&matRotY,-Rotate+4.71);// rotate the model
	D3DXMatrixRotationX(&matRotX,4.71);// the model normaly lies down so we need to turn him 90 degree to normal(4.71)
	D3DXMatrixTranslation(&matTranslation, Position.x,Position.y,Position.z); //set its position in the world
	D3DXMatrixScaling(&matScale, 0.004f, 0.004f, 0.004f); //and reduse it size to normal
	d3ddev->SetTransform(D3DTS_WORLD,&( matScale* matRotY * matTranslation) );
	Mesh->Draw();
}

Enemy_1::~Enemy_1(){
	
}