#pragma once

using namespace DirectX;

class ImmediateRenderer
{
private:
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;

	static shared_ptr<ImmediateRenderer> Instance;
	
	Model* Screen = nullptr;
	Pass* ScreenPass = nullptr;

	vector<RTTexture2D*> Textures2D;
	vector<RTTexture2D*> GBuffer;
	DSTexture2D* GDepth = nullptr;
	vector<RTTexture3D*> Textures3D;
	
	vector<DSTexture2D*> Depths;

	vector<D3D11_VIEWPORT> Viewports;
	vector<ComPtr<ID3D11RasterizerState>> RStates;

	D3D11_RASTERIZER_DESC RasterizerDesc{};


public:

	static ImmediateRenderer& GetInstance()
	{
		if (!Instance)
		{
			Instance = make_shared<ImmediateRenderer>();
			Instance->Context = D3DHardware::GetInstance().GetContext();
			Instance->SwapChain = D3DHardware::GetInstance().GetSwapChain();

			Instance->RasterizerDesc = CD3D11_RASTERIZER_DESC();
			Instance->RasterizerDesc.FillMode = D3D11_FILL_SOLID;
			Instance->RasterizerDesc.CullMode = D3D11_CULL_BACK;
			Instance->RasterizerDesc.DepthClipEnable = true;

		
		}
		return *Instance;
	}
	void ClearGBuffer(XMVECTORF32 ClearColor);
	
	void ClearTexture(RTTexture2D* Target, XMVECTORF32 ClearColor);
	void ClearDepthStencil(DSTexture2D* Target);

	void ClearTextureArray(RTTexture2DArray* Target, XMVECTORF32 ClearColor);
	void ClearDepthStencilArray(DSTexture2DArray* Target);

	void SetRenderTarget(RTTexture2D** Target, UINT Count, DSTexture2D* DepthStencil);
	void SetRenderTargetArray(RTTexture2DArray* Target, UINT Count, DSTexture2D* DepthStencil);
	void UnsetRenderTarget();

	void DrawScreen();

	RTTexture2D* GetBufferFromSwapChain();
	inline RTTexture2D** const GetGBuffer() { return GBuffer.data(); }
	inline void GetGBufferAndCount(RTTexture2D** ppOutTex, UINT* pOutCount)
	{
		ppOutTex = GBuffer.data();
		*pOutCount = GBuffer.size();
	}
	HRESULT GenerateGBuffers();


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
	
	auto GetRSDesc() { return RasterizerDesc; }

	inline ID3D11RasterizerState* const GenerateRasterizerState(D3D11_RASTERIZER_DESC& RSDesc)
	{
		ComPtr<ID3D11RasterizerState> RS;
		static auto Device = D3DHardware::GetInstance().GetDevice();

		auto Result = Device->CreateRasterizerState(&RSDesc, RS.GetAddressOf());
		ResultLog(Result, "Creating rasterizer state.");

		RStates.emplace_back(RS);

		return RStates[RStates.size() - 1].Get();

	}

	inline RTTexture2D** const GetTextures2D() { return Textures2D.data(); }
	inline RTTexture3D** const GetTextures3D() { return Textures3D.data(); }

	inline DSTexture2D** const GetDepthStencils() { return Depths.data(); }
	inline DSTexture2D* const GetGDepth() { return GDepth; }

	inline void AddViewport(D3D11_VIEWPORT Viewport) { Viewports.emplace_back(Viewport); Context->RSSetViewports(Viewports.size(), Viewports.data()); }
	inline void RemoveViewport(UINT Index) { Viewports.erase(Viewports.begin() + Index); }
	inline D3D11_VIEWPORT& GetViewport(UINT Index) { if (Index >= 0 && Index < Viewports.size()) return Viewports[Index]; return Viewports[0]; }

	inline void SwapFrame() { SwapChain->Present(0, 0); }
};