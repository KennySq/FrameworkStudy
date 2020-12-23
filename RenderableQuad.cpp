#include "stdafx.h"
#include "RenderableQuad.h"

RenderableQuad::RenderableQuad()
{
	auto IR = ImmediateRenderer::GetInstance();
	auto Memory = MemoryBank::GetInstance();

	AddComponent<Transform>();
	MR = AddComponent<MeshRenderer>();
	GenerateQuad(MR->RenderModel);

	IR.GetGBufferAndCount(GBufferRef, &GBufferCount);
	MR->Materials.emplace_back(Memory->GetMaterialByPass("Quad"));

}

RenderableQuad::~RenderableQuad()
{
}

void RenderableQuad::Init()
{
	MR->SetPass("Quad");

	for (int i = 0; i < GBufferCount; i++)
		MR->BindSRV(GBufferRef[i]->SRV.Get(), i);


}

void RenderableQuad::Update(float Delta)
{
	static auto Context = D3DHardware::GetInstance().GetContext();

	Context->PSSetShaderResources(0, 1, MR->CurrentPass->RegisterT[0].GetAddressOf());

}
