#include "stdafx.h"
#include "SkullObject.h"


void SkullObject::Init()
{
	static auto TRS = GetComponent<Transform>();
	MR = GetComponent<MeshRenderer>();
	static auto IR = ImmediateRenderer::GetInstance();
	static auto Memory = MemoryBank::GetInstance();

	TRS->Translation(XMVectorSet(0.0f,-10.0f, 0.0f, 1.0f), true);

	TRS->Rotate(XMVectorSet(0.0f, 0.0f, -90.0f, 1.0f));
	TRS->Rotate(XMVectorSet(0.0f, 11.0f, 0.0f, 1.0f));

	static auto RSD = IR.GetRSDesc();
	MR->AddRS(RSD);

	MR->SetPass("SkullObject");
	MR->BindSRV(Tex.SRV.Get(), 0);

	

}

void SkullObject::Update(float Delta)
{
	static auto Context = D3DHardware::GetInstance().GetContext();

	static auto TRS = GetComponent<Transform>();
	static auto CurrentPass = MR->CurrentPass;

	TRS->Rotate(XMVectorSet(0.0f, 0.1f*Delta, 0.0f, 1.0f));


	Context->PSSetShaderResources(0, 1, CurrentPass->RegisterT[0].GetAddressOf());

}

SkullObject::SkullObject()
{
	AddComponent<Transform>();
	MR = AddComponent<MeshRenderer>();
	auto Memory = MemoryBank::GetInstance();

	LoadStaticModelFromFile("Assets/Skull/SkullV.obj", MR->RenderModel);
	//GenerateSphere(10.0f, 32, 64, MR->RenderModel);

	LoadCubemapFromDDS("Assets/Textures/SampleCubemap.dds", &Tex);

	MR->Materials.emplace_back(Memory->GetMaterialByPass("SkullObject")); 
}


SkullObject::~SkullObject()
{
}
