#include"stdafx.h"
#include"Transform.h"

void Transform::Translation(XMVECTOR Vector)
{
	auto Origin = XMLoadFloat4x4(&TRS);

	Origin *= XMMatrixTranslationFromVector(Vector);

	XMStoreFloat4x4(&TRS, Origin);

	return;
}


void Transform::Init()
{
	D3D11_BUFFER_DESC Desc{};
	auto Device = D3DHardware::GetInstance().GetDevice();

	Desc.ByteWidth = sizeof(InstanceData);
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	auto Result = Device->CreateBuffer(&Desc, nullptr, TRSBuffer.GetAddressOf());
	ResultLog(Result, "Creating transform buffer.");

	return;
}

void Transform::Update()
{
	static auto Context = D3DHardware::GetInstance().GetContext();
	Context->UpdateSubresource(TRSBuffer.Get(), 0, nullptr, &TRS, 0, 0);
}

void Transform::Release()
{}
