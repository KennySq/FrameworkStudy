#pragma once

class ImmediateRenderer : IRenderer
{
private:
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	static shared_ptr<ImmediateRenderer> Instance;

	vector<RTTexture2D*> Textures2D;
	vector<RTTexture3D*> Textures3D;
	
	vector<DSTexture2D*> Depths;

	vector<D3D11_VIEWPORT> Viewports;

public:

	static ImmediateRenderer& GetInstance()
	{
		if (!Instance)
		{
			Instance = make_shared<ImmediateRenderer>();
			Instance->Context = D3DHardware::GetInstance().GetContext();
			Instance->SwapChain = D3DHardware::GetInstance().GetSwapChain();
		}
		return *Instance;
	}
	
	
	virtual void ClearTexture(RTTexture2D * Target, XMVECTORF32 ClearColor) override;
	void ClearDepthStencil(DSTexture2D* Target);

	RTTexture2D* GetBufferFromSwapChain();

	inline void AddTexture2D(RTTexture2D* Texture)
	{
		if (!Texture)
			AssertCritical("The texture was invalid.", E_INVALIDARG);

		Textures2D.emplace_back(Texture);
	}

	inline void AddTexture3D(RTTexture3D* Texture)
	{
		if (!Texture)
			AssertCritical("The texture was invalid.", E_INVALIDARG);

		Textures3D.emplace_back(Texture);
	}

	inline void AddDepthTexture(DSTexture2D* Texture)
	{
		if (!Texture)
			AssertCritical("The texture was invalid", E_INVALIDARG);

		Depths.emplace_back(Texture);
	}

	inline RTTexture2D** const GetTextures2D() { return Textures2D.data(); }
	inline RTTexture3D** const GetTextures3D() { return Textures3D.data(); }

	inline DSTexture2D** const GetDepthStencils() { return Depths.data(); }

	inline void AddViewport(D3D11_VIEWPORT Viewport) { Viewports.emplace_back(Viewport); Context->RSSetViewports(Viewports.size(), Viewports.data()); }
	inline void RemoveViewport(UINT Index) { Viewports.erase(Viewports.begin() + Index); }


	inline void SwapFrame() { SwapChain->Present(0, 0); }


	// IRenderer을(를) 통해 상속됨
	virtual void SetRenderTarget(RTTexture2D ** Target, UINT Count, DSTexture2D* DepthStencil) override;
	virtual void UnsetRenderTarget() override;
};