#include "stdafx.h"
#include "Skybox.h"


void Skybox::Init()
{
	MR = GetComponent<MeshRenderer>();
	TRS = GetComponent<Transform>();
	
	static auto Memory = MemoryBank::GetInstance();
	static auto HW = D3DHardware::GetInstance();
	static auto Device = HW.GetDevice();
	static auto IR = ImmediateRenderer::GetInstance();
	auto RSD = IR.GetRSDesc();

	RSD.CullMode = D3D11_CULL_NONE;

	MR->AddRS(RSD);

	
	MR->Materials.emplace_back(Memory->GetMaterialByPass("Skybox"));

	MR->SetPass("Skybox");
	MR->BindSRV(Tex.SRV.Get(), 0);

}

void Skybox::Update(float Delta)
{

	static auto Context = D3DHardware::GetInstance().GetContext();
	static auto CurrentPass = MR->CurrentPass;
	static auto Cam = GetScene()->GetMainCamera();
	static auto Trs = TRS;

	TRS->SetPosition(Cam->GetPosition());

	Context->PSSetShaderResources(0, 1, CurrentPass->RegisterT[0].GetAddressOf());
	
	


}

void Skybox::Render(float Delta)
{
	static auto Context = D3DHardware::GetInstance().GetContext();
	static ID3D11ShaderResourceView* NullSRV[] = { nullptr };

	Context->PSSetShaderResources(0, 1, NullSRV);
	
}

Skybox::Skybox()
{
	AddComponent<Transform>();
	auto MR = AddComponent<MeshRenderer>();
	
	GenerateSphere(100.0f, 32, 64, MR->RenderModel);
	LoadCubemapFromDDS("Assets/Textures/SampleCubemap.dds", &Tex);


	

}


Skybox::~Skybox()
{}
