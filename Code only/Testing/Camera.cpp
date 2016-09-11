#include "Globalincludes.h"
#include "Camera.h"
#include "Entity.h"
#include "math.h"
#include "Game.h"

Camera::Camera(){

	SensitivityX=0.5f;
	SensitivityY=0.5f;
	RotateX=0;
	RotateY=0;
	attached = false;
	attached_range =5.0f;
	C_Position = D3DXVECTOR3(-50.0f,50.0f,-50.0f);
	C_LookAtPos = D3DXVECTOR3(50.0f,0.0f,50.0f);
	C_UpVect = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

void Camera::Update(LPDIRECT3DDEVICE9& d3ddev){
	if(attached) Attach_rotate();
	else Rotate();
	D3DXMATRIX	m_matView;
	D3DXMatrixLookAtLH(&m_matView,&C_Position,&C_LookAtPos,&C_UpVect);
	d3ddev->SetTransform(D3DTS_VIEW,&m_matView);

}

void Camera::Attach(Entity* entity_){
	attached = true;
	attached_object = entity_;
	C_UpVect = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

void Camera::Move(D3DXVECTOR3 Direction){
	
	D3DXVec3Normalize(&Direction,&Direction);
	  C_Position += Direction * 0.5f;
	  C_LookAtPos += Direction * 0.5f;
	
}

void Camera::Attach_rotate(){
	//set the camera at the object that the camera is atached
	
	if(C_Position.y>Game::terrain->Get_height(C_Position.x,C_Position.z)+0.05f && attached_range<5.0f)attached_range+=0.05f;
	do{
	if(C_Position.y<Game::terrain->Get_height(C_Position.x,C_Position.z)) attached_range-=0.1f;
	C_Position = attached_object->Position;
	C_Position.x -= attached_range;
	C_Position.y += 1;	
	C_LookAtPos = attached_object->Position;
	C_LookAtPos.y += 1;
	/*if(C_Position.y<=Game::terrain->Set_height(C_Position.x,C_Position.z)){
		do{
			attached_range-=0.01f;
			C_Position = attached_object->Position;
			C_Position.x -= attached_range;
			C_Position.y += 1;
			if(attached_range<1.0f){
				C_Position.y=Game::terrain->Set_height(C_Position.x,C_Position.z);
				break;
			}
		}while(C_Position.y<=Game::terrain->Set_height(C_Position.x,C_Position.z));
	}
	if(attached_range<5.0f)attached_range+=0.01f;*/

	//Rotate the camera around the object
	if(RotateX > 1.5) RotateX=1.5; //make sure the camera will not move over 90 degree
	if(RotateX <-1.5) RotateX=-1.5;
	D3DXVECTOR3 vVector = C_Position - C_LookAtPos;
	RotateY =-RotateY;
	RotateX =-RotateX;
	C_Position.y = (float)(C_LookAtPos.y + cos(RotateX)*vVector.y - sin(RotateX)*vVector.x);
	C_Position.z = (float)(C_LookAtPos.z + (sin(RotateY)*vVector.x + cos(RotateY)*vVector.z)*cos(RotateX));
	C_Position.x = (float)(C_LookAtPos.x + (cos(RotateY)*vVector.x - sin(RotateY)*vVector.z)*cos(RotateX));
	RotateY =-RotateY;
	RotateX =-RotateX;
	}while(C_Position.y<Game::terrain->Get_height(C_Position.x,C_Position.z));
}

void Camera::Rotate(){
	D3DXVECTOR3 Direction,RotAxis;
	D3DXMATRIX matRotAxis,matRotY;
				
	D3DXVec3Normalize(&Direction,&(C_LookAtPos - C_Position));
	D3DXVec3Cross(&RotAxis,&Direction,&C_UpVect);
	D3DXVec3Normalize(&RotAxis,&RotAxis);

	D3DXMatrixRotationAxis(&matRotAxis,&RotAxis,SensitivityX*RotateX);
	D3DXMatrixRotationY(&matRotY,SensitivityY*RotateY);

	D3DXVec3TransformCoord(&Direction,&Direction,&(matRotAxis * matRotY));
	D3DXVec3TransformCoord(&C_UpVect,&C_UpVect,&(matRotAxis * matRotY));
	C_LookAtPos = Direction + C_Position;
	RotateX=0;
	RotateY=0;
}