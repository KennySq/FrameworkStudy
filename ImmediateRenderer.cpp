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

void ImmediateRenderer::SetRenderTarget(RTTexture2D ** Target, UINT Count, DSTexture2D* DepthStencil)
{
	UnsetRenderTarget();

	auto Context = D3DHardware::GetInstance().GetContext();
	
	ID3D11RenderTargetView* RTVs[8];

	for (int i = 0; i < Count; i++)
		RTVs[i] = Target[i]->RTV.Get();

	Context->OMSetRenderTargets(Count, RTVs, DepthStencil->DSV.Get());
}

void ImmediateRenderer::UnsetRenderTarget()
{
	ID3D11RenderTargetView* const UnsetRTV[] = { nullptr };
	
	Context->OMSetRenderTargets(1, UnsetRTV, nullptr);
}
