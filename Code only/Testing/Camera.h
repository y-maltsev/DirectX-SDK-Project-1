#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Entity.h"
class Camera{

	float SensitivityX;
	float SensitivityY;

public:

	float RotateX,RotateY;
	float attached_range;
	bool attached;
	Entity* attached_object;
	D3DXVECTOR3 C_Position, 
				C_LookAtPos,
				C_UpVect; //the camera "up" direction
	
	
	Camera();
	void Attach(Entity* entity); // atach the camera to Entity object
	void Attach_rotate(); // rotate the camera around the object that the camera is atached
	void Rotate(); // rotate when the camera isnt atached
	void Move(D3DXVECTOR3 Direction);
	void Update(LPDIRECT3DDEVICE9& d3ddev); 
};

#endif