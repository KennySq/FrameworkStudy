#include"stdafx.h"
#include"ImmediateRenderer.h"

shared_ptr<ImmediateRenderer> ImmediateRenderer::Instance = nullptr;

void ImmediateRenderer::ClearTexture(RTTexture2D * Target, XMVECTORF32 ClearColor)
{
	if (!Target || !Target->RTV)
		DebugLog(L_ERROR, "Target or Target RTV was nullptr.");
	
	Context->ClearRenderTargetView(Target->RTV.Get(), ClearColor);
}

void ImmediateRenderer::ClearDepthStencil(DSTexture2D * Target)
{
	Context->ClearDepthStencilView(Target->DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

RTTexture2D* ImmediateRenderer::GetBufferFromSwapChain()
{
	RTTexture2D* Texture = new RTTexture2D();

	auto Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D*), (void**)Texture->RawTexture.GetAddressOf());
	ResultLog(Result, "SwapChain buffer");

	GenerateRTVFromRTTexture2D(Texture->RawTexture.Get(), Texture->RTV.GetAddressOf());

	return Texture;
}

HRESULT ImmediateRenderer::GenerateGBuffers()
{
	auto HW = D3DHardware::GetInstance();
	auto Device = HW.GetDevice();

	RTTexture2D* Tex = new RTTexture2D();
	DSTexture2D* DTex = new DSTexture2D();

	const unsigned short GBufferCount = 5;
	D3D11_TEXTURE2D_DESC GBufferDescs =	{ 
		800,600, 1, GBufferCount, 
		DXGI_FORMAT_R32G32B32A32_FLOAT, 
		{1, 0}, D3D11_USAGE_DEFAULT,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0,0 
	};

	D3D11_TEXTURE2D_DESC DepthDescs =
	{
		800, 600, 1, GBufferCount,
		DXGI_FORMAT_R24G8_TYPELESS, {1, 0}, D3D11_USAGE_DEFAULT,
		D3D11_BIND_DEPTH_STENCIL, 0,0
	};

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	SRVDesc.Texture2DArray.ArraySize = GBufferCount;
	SRVDesc.Texture2DArray.MipLevels = 1;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc{};
	RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RTVDesc.Texture2DArray.ArraySize = GBufferCount;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2DArray.ArraySize = GBufferCount;

	auto Result = Device->CreateTexture2D(&GBufferDescs, nullptr, Tex->RawTexture.GetAddressOf());
	ResultLog(Result, "Creating GBuffer array.");

	Result = Device->CreateShaderResourceView(Tex->RawTexture.Get(), &SRVDesc, Tex->SRV.GetAddressOf());
	ResultLog(Result, "Creating GBuffer SRV.");

	Result = Device->CreateRenderTargetView(Tex->RawTexture.Get(), &RTVDesc, Tex->RTV.GetAddressOf());
	ResultLog(Result, "Creating GBuffer RTV.");



	Result = Device->CreateTexture2D(&DepthDescs, nullptr, DTex->RawTexture.GetAddressOf());
	ResultLog(Result, "Creating GBuffer depth stencil array.");

	Result = Device->CreateDepthStencilView(DTex->RawTexture.Get(), &DSVDesc, DTex->DSV.GetAddressOf());
	ResultLog(Result, "Creating GBuffer depth stencil view.");

	GBuffer = Tex;
	Depths.emplace_back(DTex);

	return Result;

}

void ImmediateRenderer::SetRenderTarget(RTTexture2D ** Target, UINT Count, DSTexture2D* DepthStencil)
{
	UnsetRenderTarget();

	auto Context = D3DHardware::GetInstance().GetContext();
	
	ID3D11RenderTargetView* RTV[] = { Target[0]->RTV.Get() };

	if(DepthStencil)
		Context->OMSetRenderTargets(Count, RTV, DepthStencil->DSV.Get());
	else
		Context->OMSetRenderTargets(Count, RTV, nullptr);

}

void ImmediateRenderer::UnsetRenderTarget()
{
	ID3D11RenderTargetView* const UnsetRTV[] = { nullptr };
	
	Context->OMSetRenderTargets(1, UnsetRTV, nullptr);
}
