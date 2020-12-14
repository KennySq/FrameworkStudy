#include "stdafx.h"
#include "Skybox.h"


void Skybox::Init()
{
	static auto MR = GetComponent<MeshRenderer>();
	static auto Memory = MemoryBank::GetInstance();
	static auto HW = D3DHardware::GetInstance();
	static auto Device = HW.GetDevice();

	static auto RSD = HW.GetRSDesc();

	RSD.CullMode = D3D11_CULL_NONE;

	MR->AddRS(RSD);

	
	MR->Materials.emplace_back(Memory->GetMaterialByPass("Skybox"));

	MR->SetPass("Skybox");

}

void Skybox::Update(float Delta)
{}

Skybox::Skybox()
{
	AddComponent<Transform>();
	auto MR = AddComponent<MeshRenderer>();
	
	GenerateSphere(100.0f, 32, 64, MR->RenderModel);

	

}


Skybox::~Skybox()
{}
