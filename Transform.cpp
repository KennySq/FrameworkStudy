#include"stdafx.h"
#include"Transform.h"

void Transform::Translation(XMVECTOR Vector)
{
	auto Origin = XMLoadFloat4x4(&TRS);

	Origin *= XMMatrixTranspose(XMMatrixTranslationFromVector(Vector));
	XMStoreFloat4x4(&TRS, Origin);

	return;
}

void Transform::Rotate(XMVECTOR Vector)
{
	auto Origin = XMLoadFloat4x4(&TRS);

	Origin *= XMMatrixTranspose(XMMatrixRotationRollPitchYaw(
		Vector.m128_f32[2],
		Vector.m128_f32[0],
		Vector.m128_f32[1]
	));
	
	XMStoreFloat4x4(&TRS,Origin);

	return;
}

void Transform::SetScale(XMVECTOR Vector)
{
	TRS._11 = Vector.m128_f32[0];
	TRS._22 = Vector.m128_f32[1];
	TRS._33 = Vector.m128_f32[2];
}

void Transform::SetScale(float x, float y, float z)
{
	TRS._11 = x;
	TRS._22 = y;
	TRS._33 = z;
}

void Transform::SetPosition(float x, float y, float z)
{

	TRS._41 = x;
	TRS._42 = y;
	TRS._43 = z;
}

void Transform::SetPosition(XMVECTOR Vector)
{
	TRS._14 = Vector.m128_f32[0];
	TRS._24 = Vector.m128_f32[1];
	TRS._34 = Vector.m128_f32[2];
}

void Transform::Orbiting(XMVECTOR Point, float Distance, float Theta)
{
	auto Origin = XMLoadFloat4x4(&TRS);

	Origin.r[0].m128_f32[3] = Point.m128_f32[0]  + sin(Theta);
	Origin.r[1].m128_f32[3] = Point.m128_f32[1];
	Origin.r[2].m128_f32[3] = Point.m128_f32[2] + cos(Theta);
	Origin.r[3].m128_f32[3] = Point.m128_f32[3];

	Origin.r[0].m128_f32[3] *= Distance;
//	Origin.r[1].m128_f32[3] *= Distance;
	Origin.r[2].m128_f32[3] *= Distance;

	XMStoreFloat4x4(&TRS, XMMatrixTranspose(Origin));

	return;
}

void Transform::Init()
{
	D3D11_BUFFER_DESC Desc{};
	auto Device = D3DHardware::GetInstance().GetDevice();
	auto Context = D3DHardware::GetInstance().GetContext();

	Desc.ByteWidth = sizeof(InstanceData);
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	auto Result = Device->CreateBuffer(&Desc, nullptr, TRSBuffer.GetAddressOf());
	ResultLog(Result, "Creating transform buffer.");

	XMStoreFloat4x4(&TRS, XMMatrixIdentity());
	Context->UpdateSubresource(TRSBuffer.Get(), 0, nullptr, &TRS, 0, 0);

	return;
}

void Transform::Update()
{
	static auto Context = D3DHardware::GetInstance().GetContext();
	
//	swap(TRS._14, TRS._41);
//	swap(TRS._24, TRS._42);
//	swap(TRS._34, TRS._43);
	
	Context->UpdateSubresource(TRSBuffer.Get(), 0, nullptr, &TRS, 0, 0);
}

void Transform::Release()
{}
