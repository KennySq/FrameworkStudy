#include"stdafx.h"
#include"Camera.h"

void Camera::Init()
{
	D3D11_BUFFER_DESC Desc{};
	D3D11_SUBRESOURCE_DATA Sub{};

	auto Device = D3DHardware::GetInstance().GetDevice();

	auto V = XMMatrixLookAtLH(XMVectorSet(20.0f, 20.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	auto P = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.333f, 0.01f, 100.0f);

	SetView(XMMatrixTranspose(V));
	SetProjection(XMMatrixTranspose(P));

	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.ByteWidth = sizeof(XMFLOAT4X4) * 2;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	XMMATRIX Mat[] = { XMMatrixTranspose(V),XMMatrixTranspose(P) };

	Sub.pSysMem = &Mat;

	auto Result = Device->CreateBuffer(&Desc, &Sub, GetBuffer().GetAddressOf());
	ResultLog(Result, "Creating camera buffer");



	auto RootScene = Root->GetScene();
	RootScene->SetMainCamera(this);

}

void Camera::Update()
{}

void Camera::Release()
{}
