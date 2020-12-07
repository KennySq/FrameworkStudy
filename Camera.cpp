#include"stdafx.h"
#include"Camera.h"

void Camera::Init()
{
	D3D11_BUFFER_DESC Desc{};
	D3D11_SUBRESOURCE_DATA Sub{};

	auto Device = D3DHardware::GetInstance().GetDevice();

	auto V = XMMatrixLookAtLH(XMVectorSet(-40.0f, 0.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	auto P = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.333f, 0.01f, 100.0f);

	SetView(XMMatrixTranspose(V));
	SetProjection(XMMatrixTranspose(P));

	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.ByteWidth = sizeof(CameraInfo);
	Desc.Usage = D3D11_USAGE_DEFAULT;

	CameraInfo Info;
	
	XMStoreFloat4x4(&Info.View, XMMatrixTranspose(V));
	XMStoreFloat4x4(&Info.Projection, XMMatrixTranspose(P));
	XMStoreFloat4(&Info.ViewPosition, GetViewPosition());

	Sub.pSysMem = &Info;

	auto Result = Device->CreateBuffer(&Desc, &Sub, GetBuffer().GetAddressOf());
	ResultLog(Result, "Creating camera buffer");



	auto RootScene = Root->GetScene();
	RootScene->SetMainCamera(this);

}

void Camera::Update()
{}

void Camera::Release()
{}
