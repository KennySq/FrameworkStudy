#include"stdafx.h"
#include"Camera.h"

void Camera::Init()
{
	D3D11_BUFFER_DESC Desc{};

	auto Device = D3DHardware::GetInstance().GetDevice();

	auto V = XMMatrixLookAtLH(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
							  XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	auto P = XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0f), 1.333f, 0.01f, 100.0f);

	SetView(XMMatrixTranspose(V));
	SetProjection(XMMatrixTranspose(P));

	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.ByteWidth = sizeof(XMFLOAT4X4) * 2;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	auto Result = Device->CreateBuffer(&Desc, nullptr, GetBuffer().GetAddressOf());
	ResultLog(Result, "Creating camera buffer");


}

void Camera::Update()
{}

void Camera::Release()
{}
