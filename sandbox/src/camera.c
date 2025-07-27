#include <input.h>

#include "camera.h"

Camera3D thisCamera = 
{
	.x = 0.0f,
	.y = 0.0f,
	.z = 0.0f,
	.yaw = 0.0f,
	.pitch = 0.0f
};

void cameraUpdate(float delta) 
{
   	if(rightDown) 
	{
		thisCamera.yaw += 0.5f;
	} else if (leftDown)
	{
		thisCamera.yaw -= 0.5f;
	}
	
	if(upDown) 
	{
		thisCamera.pitch -= 0.5f;
	} else if (downDown)
	{
		thisCamera.pitch += 0.5f;
	}
}