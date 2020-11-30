#pragma once

using namespace DirectX;

struct IRenderer
{
	virtual ~IRenderer() {}

	virtual void ClearTexture(RTTexture2D* Target, XMVECTORF32 ClearColor) = 0;
	
	virtual void SetRenderTarget(RTTexture2D** Target, UINT Count, DSTexture2D* DepthStencil) = 0;
	virtual void UnsetRenderTarget() = 0;


	// ID3D11RasterizerState 는 Material 레벨에서 관리
};