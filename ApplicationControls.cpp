#include"stdafx.h"
#include"ApplicationControls.h"

void CameraForward(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(-0.1f, 0.0f, 0.0f,1.0f));
	
}

void CameraBackward(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.1f, 0.0f, 0.0f, 1.0f));

}

void CameraLeft(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.0f, -0.1f, 1.0f));

}

void CameraRight(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.0f, 0.1f, 1.0f));

}

void CameraUp(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, -0.1f, 0.0f, 1.0f));

}

void CameraDown(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.1f, 0.0f, 1.0f));

}

void CameraRotatePitch(Camera * const Cam, int dx, int dy)
{
	DebugLog(L_NORMAL, "Rotating pitch by mouse." + to_string(dx) + ", " + to_string(dy));
	Cam->Rotate(XMVectorSet(0.0f, (float)dy*0.1f, 0.0f, 1.0f));

}

void CameraRotateYaw(Camera * const Cam, int dx, int dy)
{
	DebugLog(L_NORMAL, "Rotating yaw by mouse." + to_string(dx) + ", " + to_string(dy));
	Cam->Rotate(XMVectorSet(0.0f, 0.0f, (float)dx*0.1f,1.0f));
}
