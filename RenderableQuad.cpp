#include "stdafx.h"
#include "RenderableQuad.h"

RenderableQuad::RenderableQuad()
{

	TRS = AddComponent<Transform>();
	MR = AddComponent<MeshRenderer>();

	QuadTex = new UATexture3D();

	auto Result = CreateUnorderedAccessTexture3D(DXGI_FORMAT::DXGI_FORMAT_R32_UINT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, 128, 128, 128, QuadTex);
	auto Memory = MemoryBank::GetInstance();

	GenerateQuad(MR->RenderModel);

	Mat = Memory->GetMaterialByPass("VolumeTexture");
	CSPass = Mat->Passes["VolumeTexture"];


	MR->Materials.emplace_back(Mat);
	MR->SetPass("VolumeTexture");

	MR->BindUAV(QuadTex->UAV.Get(), 0);

}

RenderableQuad::~RenderableQuad()
{
}

void RenderableQuad::Init()
{
	auto Context = D3DHardware::GetInstance().GetContext();
	auto CS = CSPass->CS.Get();

	Context->CSSetShader(CS, nullptr, 0);
	Context->CSSetUnorderedAccessViews(0, 1, CSPass->RegisterU[0].GetAddressOf(), nullptr);

	Context->Dispatch(32, 32, 32);

	DebugLog(L_NORMAL, "Renderable Quad, compute shader has been dispatched.");


}

void RenderableQuad::Update(float Delta)
{

}

void RenderableQuad::Render(float Delta)
{
}

void RenderableQuad::Release()
{
}


