#include"stdafx.h"
#include"ApplicationControls.h"

void CameraForward(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(-0.1f, 0.0f, 0.0f,1.0f), false);
	
}

void CameraBackward(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.1f, 0.0f, 0.0f, 1.0f),false);

}

void CameraLeft(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.0f, -0.1f, 1.0f),false);

}

void CameraRight(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.0f, 0.1f, 1.0f),false);

}

void CameraUp(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, -0.1f, 0.0f, 1.0f),false);

}

void CameraDown(Camera * const Cam)
{
	Cam->Translation(XMVectorSet(0.0f, 0.1f, 0.0f, 1.0f),false);

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
	Cam->Rotate(XMVectorSet(delta * 0.01f, 0.0f, 0.0f, 0.0f));

	//Cam->Orbiting(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 10.0f, (float)delta*0.0001f);
}

void CameraRotateOrbitPitch(Camera * const Cam, int delta)
{

	Cam->Rotate(XMVectorSet(0.0f, delta * 0.01f, 0.0f, 0.0f));
//	Cam->Orbiting(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 2.0f, (float)delta*0.1f);

}
