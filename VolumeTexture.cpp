#include "stdafx.h"
#include "VolumeTexture.h"

VolumeTexture::VolumeTexture()
{

	TRS = AddComponent<Transform>();

	QuadTex = new UATexture3D();

	auto Result = CreateUnorderedAccessTexture3D(DXGI_FORMAT::DXGI_FORMAT_R32_UINT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 128, 128, 128, QuadTex);
	auto Memory = MemoryBank::GetInstance();

	CO = Memory->GetComputeObjectByPass("VolumeTexture");

	CO->BindUAV(QuadTex->UAV.Get(), 0);
}

VolumeTexture::~VolumeTexture()
{
}

void VolumeTexture::Init()
{


}

void VolumeTexture::Update(float Delta)
{
	//auto Context = D3DHardware::GetInstance().GetContext();
	//auto CS = CO->CS.Get();
	//static ID3D11UnorderedAccessView* NullUAV[] = { nullptr };

	//Context->CSSetShader(CS, nullptr, 0);
	//Context->CSSetUnorderedAccessViews(0, 1, CO->RegisterU[0].GetAddressOf(), nullptr);

	//Context->Dispatch(32, 32, 32);

	//DebugLog(L_NORMAL, "Renderable Quad, compute shader has been dispatched.");
	//Context->CSSetUnorderedAccessViews(0, 1, NullUAV, nullptr);

}

void VolumeTexture::Render(float Delta)
{
}

void VolumeTexture::Release()
{
}

