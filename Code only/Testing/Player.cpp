#include "Globalincludes.h"
#include "Player.h"
#include "Game.h"

int Player::lifes=5;

Player::Player(LPDIRECT3DDEVICE9& d3ddev){
	//make new model
	Mesh = new CModel(d3ddev);
	//Load the new model
	Mesh->LoadXFile("US Ranger.x");

	//set the animation to standing position
	Mesh->SetCurrentAnimation(8); //ajust the legs
		Mesh->Update(); //make an update so it can remember the legs' positon
	Mesh->SetCurrentAnimation(9); //ajust the arms
	Position.x=5.0f;
	Position.z=5.0f;
	Rotate=0.0f;
	health=250;
	jump_fall_speed=0;
	Score=0;
	moving_forward=false;
	moving_backwards=false;
	moving_left=false;
	moving_right=false;
}

void Player::Update(D3DXVECTOR3 C_Position,D3DXVECTOR3 C_LookAtPos){
	if(health<=0) { //check is the player dead
		for(unsigned int i=0;i<Game::enemyes.size();i++)//remove the player as target from the enemyes
			if(Game::enemyes[i]->target==this) Game::enemyes[i]->target=NULL;
		delete this; // release the player
		Game::player=NULL; // ensure that the player is released
		Game::camera->attached=false; //release the camera
		return; //break the update function
	}
	Move(C_Position, C_LookAtPos);

	//player's gravitation
	float normal_hight = Game::terrain->Get_height(Position.x,Position.z)+0.8f;
	jump_fall_speed-=0.05f;
	Position.y +=jump_fall_speed;
	if(Position.y<=normal_hight){
		jump_fall_speed=0;
		Position.y=normal_hight;
	}
	Move_animation();
}
void Player::Move_animation(){
	if(moving_left)Mesh->SetCurrentAnimation(5); //move left animation
	if(moving_right)Mesh->SetCurrentAnimation(6); // move right animation
	if(moving_forward){
		Mesh->SetCurrentAnimation(7); // move forward animation
	}
	if(moving_backwards){
		if(!Mesh->m_dAnimationReverse){ // the model dont have backwards movement - so it needs the normal forward movement with in reverse
			
			Mesh->m_dAnimationReverse = true; // set reverse animation
			Mesh->Time=0; //+the 2 next commands - set the time to reverse by reseting the time in the animation controler and placing the old time
			Mesh->m_pAnimController->ResetTime(); //
			Mesh->m_pAnimController->AdvanceTime(Reverse_time,NULL);
		}
		// since reversing needs 2 seconds to renew we save the old time(after these 2 seconds), so when we switch to reverse we dont get every time 2 seconds renew time
		Reverse_time = Mesh->m_pAnimController->GetTime(); 
		Mesh->SetCurrentAnimation(7); // move forward animation - that will look like moving backward
	}
	if(Mesh->m_dAnimationReverse && !moving_backwards){ // check is the animation time reverse and turn it back normal if the player is no longer moving back
			//if its reverse, set it back to normal
			Mesh->Time=0;
			Mesh->m_pAnimController->ResetTime();
			Mesh->m_dAnimationReverse = false;	
	}
	if(!(moving_forward || moving_left || moving_right || moving_backwards) || // check is the player standing still
		((moving_backwards && moving_forward) || ( moving_left && moving_right))){ //or he's making 2 actions making each other irelevent
		Mesh->SetCurrentAnimation(8); //standing position animation
	}
}
void Player::Move(D3DXVECTOR3 C_Position,D3DXVECTOR3 C_LookAtPos){
	D3DXVECTOR3 Direction(0,0,0);
	D3DXVECTOR3 C_UpVect(0,1,0);

	float speed_limit=1.0f; // decrease the speed to normal if moving in the same time forward or backward and left or right
	if((moving_forward || moving_backwards) && (moving_left || moving_right)) speed_limit = 0.707f;

	if(moving_forward) 
	{
		Direction=C_LookAtPos - C_Position;
		float normalize_ = 1/sqrt((Direction.x *Direction.x) + (Direction.z *Direction.z));// the x,z can be close to 0 so we need to multiply them so the total speed from the 2 can be normal(1)
		Direction.y *=0; //the camera moves the player in all 3 cordinates - so we need to remove the vertical one
		Direction.x *=normalize_;
		Direction.z *=normalize_;
		D3DXVec3Normalize(&Direction,&Direction);
		Position += Direction * 0.2f * speed_limit;
	}
	if(moving_backwards)
	{
		Direction=C_Position - C_LookAtPos;
		float normalize_ = 1/sqrt((Direction.x * Direction.x) + (Direction.z * Direction.z)); // the x,z can be close to 0 so we need to multiply them so the total speed from the 2 can be normal(1)
		Direction.y *=0; //the camera moves the player in all 3 cordinates - so we need to remove the vertical one
		Direction.x *=normalize_; 
		Direction.z *=normalize_;
		D3DXVec3Normalize(&Direction,&Direction);
		Position += Direction * 0.2f * speed_limit;
	}
	if(moving_left)
	{
		D3DXVec3Cross(&Direction,&(C_LookAtPos - C_Position),&C_UpVect);
		D3DXVec3Normalize(&Direction,&Direction);
		Position += Direction * 0.2f*speed_limit;
	}
	if(moving_right)
	{
		D3DXVec3Cross(&Direction,&(C_Position- C_LookAtPos ),&C_UpVect);
		D3DXVec3Normalize(&Direction,&Direction);
		Position += Direction * 0.2f * speed_limit;
	}
	
}
void Player::Draw(LPDIRECT3DDEVICE9 &d3ddev){
	D3DXMATRIX  matScale,matTranslation,matRotY,matRotX;
	D3DXMatrixRotationY(&matRotY,Rotate+1.57); // rotate the model so it can face the same way as the camera
	D3DXMatrixRotationX(&matRotX,4.71);// the player's model is normaly lie down so we need to turn him 90 degree to normal
	D3DXMatrixTranslation(&matTranslation, Position.x,Position.y,Position.z);   //set the position
	D3DXMatrixScaling(&matScale, 0.04f, 0.04f, 0.04f); //and reduce the size of the model to normal
	// tell Direct3D about our matrixs
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale  * matRotY  * matTranslation));
	Mesh->Update();// update the animation before draw
	Mesh->Draw(); 


}

Player::~Player(){
	SAFE_DELETE(Mesh);
}