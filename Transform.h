#pragma once

struct InstanceData
{
	XMFLOAT4X4 WorldTransform;
};

struct Transform : Component
{
private:
	XMFLOAT4X4 TRS;

	XMFLOAT4 Position;
	XMFLOAT4 Rotation;
	XMFLOAT4 Scale;

	ComPtr<ID3D11Buffer> TRSBuffer;

public:
	inline XMMATRIX const GetMatrix() { return XMLoadFloat4x4(&TRS); }
	
	inline XMVECTOR const GetPosition() { return XMLoadFloat4(&Position); }
	inline XMVECTOR const GetRotation() { return XMLoadFloat4(&Rotation); }
	inline XMVECTOR const GetScale() { return XMLoadFloat4(&Scale); }

	inline ID3D11Buffer* const GetBuffer() { return TRSBuffer.Get(); }

	void Translation(XMVECTOR Vector);
	void Rotate(XMVECTOR Vector);

	// Component을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;

};