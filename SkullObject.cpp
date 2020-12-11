#include "stdafx.h"
#include "SkullObject.h"


void SkullObject::Init()
{
	static auto TRS = GetComponent<Transform>();
	TRS->Translation(XMVectorSet(0.0f,0.0f, 0.0f, 1.0f));

	TRS->Rotate(XMVectorSet(0.0f, 0.0f, -90.0f, 1.0f));
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

	Model* M = new Model();
	Material* Mat = new Material();

	LoadStaticModelFromFile("Assets/Skull/SkullV.obj", M);
	MR->SetModel(M);

	Mat->AddPass("Assets/Shaders/SampleShader.hlsl", "Sample");
	Memory->AddMaterial(Mat);

	MR->Materials.emplace_back(Mat); 
}


SkullObject::~SkullObject()
{}
