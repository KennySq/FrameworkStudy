#include "stdafx.h"
#include "SkullObject.h"


void SkullObject::Init()
{
	static auto TRS = GetComponent<Transform>();
	static auto MR = GetComponent<MeshRenderer>();
	static auto HW = D3DHardware::GetInstance();

	TRS->Translation(XMVectorSet(0.0f,0.0f, 0.0f, 1.0f));

	TRS->Rotate(XMVectorSet(0.0f, 0.0f, -90.0f, 1.0f));

	static auto RSD = HW.GetRSDesc();
	MR->AddRS(RSD);

	MR->SetPass("Sample");
}

void SkullObject::Update(float Delta)
{
	static auto TRS = GetComponent<Transform>();

	TRS->Rotate(XMVectorSet(0.0f, 0.1f * Delta, 0.0f, 1.0f));

}

SkullObject::SkullObject()
{
	AddComponent<Transform>();
	auto MR = AddComponent<MeshRenderer>();
	auto Memory = MemoryBank::GetInstance();

	LoadStaticModelFromFile("Assets/Skull/SkullV.obj", MR->RenderModel);


	MR->Materials.emplace_back(Memory->GetMaterialByPass("Sample")); 
}


SkullObject::~SkullObject()
{
}
