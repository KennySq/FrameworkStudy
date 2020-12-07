#pragma once

struct CameraInfo
{
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
	XMFLOAT4 ViewPosition;
};

class Camera : Component
{
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
	float FOV;
	float AspectRatio;

	ComPtr<ID3D11Buffer> CameraBuffer;

public:
	inline const XMMATRIX GetView() { return XMLoadFloat4x4(&View); }
	inline const XMMATRIX GetProjection() { return XMLoadFloat4x4(&Projection); }

	inline const XMVECTOR GetViewPosition()
	{
		auto V = XMLoadFloat4x4(&View);
		auto Dt = XMMatrixDeterminant(V);
		auto Inv = XMMatrixInverse(&Dt, V);

		return XMVectorSet(Inv.r[3].m128_f32[0], Inv.r[3].m128_f32[1], Inv.r[3].m128_f32[2], Inv.r[3].m128_f32[4]);
	}

	inline void SetView(XMMATRIX const Mat) { XMStoreFloat4x4(&View, Mat); }
	inline void SetProjection(XMMATRIX const Mat) { XMStoreFloat4x4(&Projection, Mat); }

	inline ComPtr<ID3D11Buffer>& const GetBuffer() { return CameraBuffer; }


	// Component을(를) 통해 상속됨
	virtual void Init() override;

	virtual void Update() override;

	virtual void Release() override;

};