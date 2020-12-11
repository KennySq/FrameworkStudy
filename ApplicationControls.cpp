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

void CameraRotatePitch(Camera * const Cam, int delta)
{
	DebugLog(L_NORMAL, "Rotating pitch by mouse." + to_string(delta));
	Cam->Rotate(XMVectorSet(0.0f, (float)delta*0.01f, 0.0f, 1.0f));

}

void CameraRotateYaw(Camera * const Cam, int delta)
{
	DebugLog(L_NORMAL, "Rotating yaw by mouse." + to_string(delta));
	Cam->Rotate(XMVectorSet(0.0f, 0.0f, (float)delta*0.01f,1.0f));
}

void CameraRotateOrbitYaw(Camera * const Cam, int delta)
{
	Cam->Orbiting(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 10.0f, (float)delta*0.0001f);
}

void CameraRotateOrbitPitch(Camera * const Cam, int delta)
{
//	Cam->Orbiting(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 2.0f, (float)delta*0.1f);

}
