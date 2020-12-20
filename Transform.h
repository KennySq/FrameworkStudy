#pragma once

struct InstanceData
{
	XMFLOAT4X4 WorldTransform;
};

struct Transform : Component
{
protected:
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

	virtual void Translation(XMVECTOR Vector, bool isLocal);
	virtual void Rotate(XMVECTOR Vector);
	virtual void SetScale(XMVECTOR Vector);
	virtual void SetScale(float x, float y, float z);
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(XMVECTOR Vector);

	virtual void Orbiting(XMVECTOR Point, float Distance, float Theta);

	// Component을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;

};