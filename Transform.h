#pragma once

struct Transform : Component
{
private:
	XMFLOAT4X4 TRS;

	XMFLOAT4 Position;
	XMFLOAT4 Rotation;
	XMFLOAT4 Scale;

public:
	XMMATRIX const GetMatrix() { return XMLoadFloat4x4(&TRS); }
	
	XMVECTOR const GetPosition() { return XMLoadFloat4(&Position); }
	XMVECTOR const GetRotation() { return XMLoadFloat4(&Rotation); }
	XMVECTOR const GetScale() { return XMLoadFloat4(&Scale); }

};