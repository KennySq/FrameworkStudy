#include"stdafx.h"
#include"Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{
}

void Transform::Translation(XMVECTOR Vector, bool isLocal)
{
	auto Origin = XMLoadFloat4x4(&Inst->WorldTransform);
	
	if (isLocal)
	{
		Origin *= XMMatrixTranspose(XMMatrixTranslationFromVector(Vector));
	}
	else
	{
		Origin.r[3].m128_f32[0] += Vector.m128_f32[0];
		Origin.r[3].m128_f32[1] += Vector.m128_f32[1];
		Origin.r[3].m128_f32[2] += Vector.m128_f32[2];

	}
	XMStoreFloat4x4(&Inst->WorldTransform, Origin);

	return;
}

void Transform::Rotate(XMVECTOR Vector)
{
	auto Origin = XMLoadFloat4x4(&Inst->WorldTransform);

	Origin *= XMMatrixTranspose(XMMatrixRotationRollPitchYaw(
		Vector.m128_f32[2],
		Vector.m128_f32[0],
		Vector.m128_f32[1]
	));
	
	XMStoreFloat4x4(&Inst->WorldTransform,Origin);

	return;
}

void Transform::SetScale(XMVECTOR Vector)
{
	Inst->WorldTransform._11 = Vector.m128_f32[0];
	Inst->WorldTransform._22 = Vector.m128_f32[1];
	Inst->WorldTransform._33 = Vector.m128_f32[2];
}

void Transform::SetScale(float x, float y, float z)
{
	Inst->WorldTransform._11 = x;
	Inst->WorldTransform._22 = y;
	Inst->WorldTransform._33 = z;
}

void Transform::SetPosition(float x, float y, float z)
{

	Inst->WorldTransform._41 = x;
	Inst->WorldTransform._42 = y;
	Inst->WorldTransform._43 = z;
}

void Transform::SetPosition(XMVECTOR Vector)
{
	Inst->WorldTransform._14 = Vector.m128_f32[0];
	Inst->WorldTransform._24 = Vector.m128_f32[1];
	Inst->WorldTransform._34 = Vector.m128_f32[2];
}

void Transform::Orbiting(XMVECTOR Point, float Distance, float Theta)
{
	auto Origin = XMLoadFloat4x4(&Inst->WorldTransform);

	Origin.r[0].m128_f32[3] = Point.m128_f32[0]  + sin(Theta);
	Origin.r[1].m128_f32[3] = Point.m128_f32[1];
	Origin.r[2].m128_f32[3] = Point.m128_f32[2] + cos(Theta);
	Origin.r[3].m128_f32[3] = Point.m128_f32[3];

	Origin.r[0].m128_f32[3] *= Distance;
//	Origin.r[1].m128_f32[3] *= Distance;
	Origin.r[2].m128_f32[3] *= Distance;

	XMStoreFloat4x4(&Inst->WorldTransform, XMMatrixTranspose(Origin));

	return;
}

void Transform::Init()
{
	Inst = &Root->GetInstanceData();
	InstanceBuffer = Root->GetBuffer();
	XMStoreFloat4x4(&Inst->WorldTransform, XMMatrixIdentity());

	return;
}

void Transform::Update()
{
	static auto Context = D3DHardware::GetInstance().GetContext();
	Context->UpdateSubresource(InstanceBuffer.Get(), 0, nullptr, Inst, 0, 0);

}

void Transform::Release()
{}
