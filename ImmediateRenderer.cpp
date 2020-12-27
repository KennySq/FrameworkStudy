#include"stdafx.h"
#include"ImmediateRenderer.h"

shared_ptr<ImmediateRenderer> ImmediateRenderer::Instance = nullptr;

void ImmediateRenderer::ClearGBuffer(XMVECTORF32 ClearColor)
{
	for (int i = 0; i < GBuffer.size(); i++)
		ClearTexture(GBuffer[i], ClearColor);
}

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

void ImmediateRenderer::ClearTextureArray(RTTexture2DArray* Target, XMVECTORF32 ClearColor)
{
	if(!Target)
		DebugLog(L_ERROR, "Target or Target RTV was nullptr.");

	for (int i = 0; i < Target->RTV.size(); i++)
		Context->ClearRenderTargetView(Target->RTV[i].Get(), ClearColor);
}

void ImmediateRenderer::ClearDepthStencilArray(DSTexture2DArray* Target)
{
	if(!Target)
		DebugLog(L_ERROR, "Target or Target DSV was nullptr.");

	for (int i = 0; i < Target->DSV.size(); i++)
		Context->ClearDepthStencilView(Target->DSV[i].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

RTTexture2D* ImmediateRenderer::GetBufferFromSwapChain()
{
	auto Memory = MemoryBank::GetInstance();
	RTTexture2D* Texture = new RTTexture2D();
	Screen = new Model();

	auto Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D*), (void**)Texture->RawTexture.GetAddressOf());
	ResultLog(Result, "SwapChain buffer");

	GenerateRTVFromRTTexture2D(Texture->RawTexture.Get(), Texture->RTV.GetAddressOf());

	GenerateQuad(Screen);
	
	auto Mat = Memory->GetMaterialByPass("Quad");
	ScreenPass = Mat->Passes["Quad"];

	auto RSDesc = GetRSDesc();
	RSDesc.CullMode = D3D11_CULL_NONE;

	RStates.emplace_back(GenerateRasterizerState(RSDesc));

	return Texture;
}

HRESULT ImmediateRenderer::GenerateGBuffers()
{
	auto HW = D3DHardware::GetInstance();
	auto Device = HW.GetDevice();
	const unsigned short GBufferCount = 6;


	DSTexture2D* DTex = new DSTexture2D();


	D3D11_TEXTURE2D_DESC DepthDescs =
	{
		800, 600, 1, 1,
		DXGI_FORMAT_R24G8_TYPELESS, {1, 0}, D3D11_USAGE_DEFAULT,
		D3D11_BIND_DEPTH_STENCIL, 0,0
	};


	

	for (int i = 0; i < GBufferCount; i++)
	{
		RTTexture2D* Tex = new RTTexture2D();

		D3D11_TEXTURE2D_DESC GBufferDescs = {
			800,600, 1, 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			{1, 0}, D3D11_USAGE_DEFAULT,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0,0
		};

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
		SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc{};
		RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


		auto Result = Device->CreateTexture2D(&GBufferDescs, nullptr, Tex->RawTexture.GetAddressOf());
		ResultLog(Result, "Creating GBuffer array.");

		Result = Device->CreateShaderResourceView(Tex->RawTexture.Get(), &SRVDesc, Tex->SRV.GetAddressOf());
		ResultLog(Result, "Creating GBuffer SRV.");

		Result = Device->CreateRenderTargetView(Tex->RawTexture.Get(), &RTVDesc, Tex->RTV.GetAddressOf());
		ResultLog(Result, "Creating GBuffer RTV.");

		GBuffer.emplace_back(Tex);

	}


	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2DArray.ArraySize = 1;

	auto Result = Device->CreateTexture2D(&DepthDescs, nullptr, DTex->RawTexture.GetAddressOf());
	ResultLog(Result, "Creating GBuffer depth stencil array.");

	Result = Device->CreateDepthStencilView(DTex->RawTexture.Get(), &DSVDesc, DTex->DSV.GetAddressOf());
	ResultLog(Result, "Creating GBuffer depth stencil view.");

	GDepth = DTex;

	return Result;

}

void ImmediateRenderer::DrawScreen()
{
	static UINT Strides[] = { sizeof(StaticVertex) };
	static UINT Offsets[] = { 0 };

	static ID3D11RenderTargetView* GBufferRTV[8];
	static ID3D11ShaderResourceView* GBufferSRV[8];
	static ID3D11ShaderResourceView* NullSRV[] = { nullptr };

	Context->VSSetShader(ScreenPass->VS.Get(), nullptr, 0);
	Context->PSSetShader(ScreenPass->PS.Get(), nullptr, 0);
	
	Context->RSSetState(RStates[0].Get());

	Context->IASetVertexBuffers(0, 1, Screen->VertexBuffer.GetAddressOf(), Strides, Offsets);
	Context->IASetIndexBuffer(Screen->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Context->IASetInputLayout(ScreenPass->IL.Get());
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (UINT i = 0; i < GBuffer.size(); i++)
	{
		GBufferSRV[i] = GBuffer[i]->SRV.Get();
		GBufferRTV[i] = GBuffer[i]->RTV.Get();

	}

	Context->PSSetShaderResources(0, GBuffer.size(), GBufferSRV);
	Context->OMSetRenderTargets(1, Textures2D[0]->RTV.GetAddressOf(), nullptr);
	

	Context->DrawIndexed(6, 0, 0);
	

	Context->PSSetShaderResources(0, 1, NullSRV);
	UnsetRenderTarget();

}

void ImmediateRenderer::SetRenderTarget(RTTexture2D ** Target, UINT Count, DSTexture2D* DepthStencil)
{
	UnsetRenderTarget();

	auto Context = D3DHardware::GetInstance().GetContext();
	
	static ID3D11RenderTargetView* RTV[8];
	
	for (int i = 0; i < Count; i++)
		RTV[i] = Target[i]->RTV.Get();

	if(DepthStencil)
		Context->OMSetRenderTargets(Count, RTV, DepthStencil->DSV.Get());
	else
		Context->OMSetRenderTargets(Count, RTV, nullptr);

}

void ImmediateRenderer::SetRenderTargetArray(RTTexture2DArray* Target, UINT Count, DSTexture2D* DepthStencil)
{

	UnsetRenderTarget();

	auto Context = D3DHardware::GetInstance().GetContext();

	static ID3D11RenderTargetView* RTV[8];

	for (int i = 0; i < Count; i++)
	{
		RTV[i] = Target->RTV[i].Get();
	}

	if (DepthStencil)
		Context->OMSetRenderTargets(Count, RTV, DepthStencil->DSV.Get());
	else
		Context->OMSetRenderTargets(Count, RTV, nullptr);
}

void ImmediateRenderer::UnsetRenderTarget()
{
	ID3D11RenderTargetView* const UnsetRTV[] = { nullptr };
	
	Context->OMSetRenderTargets(1, UnsetRTV, nullptr);
}
