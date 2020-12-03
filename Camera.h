#pragma once

class Camera : Component
{
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
	float FOV;
	float AspectRatio;

	ComPtr<ID3D11Buffer> CameraBuffer;

public:
	inline constexpr XMMATRIX GetView() { return XMLoadFloat4x4(&View); }
	inline constexpr XMMATRIX GetProjection() { return XMLoadFloat4x4(&Projection); }

	inline void SetView(XMMATRIX const Mat) { XMStoreFloat4x4(&View, Mat); }
	inline void SetProjection(XMMATRIX const Mat) { XMStoreFloat4x4(&Projection, Mat); }

	inline ComPtr<ID3D11Buffer>& const GetBuffer() { return CameraBuffer; }


	// Component을(를) 통해 상속됨
	virtual void Init() override;

	virtual void Update() override;

	virtual void Release() override;

};