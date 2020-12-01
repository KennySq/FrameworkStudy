#include"stdafx.h"
#include"MeshRenderer.h"

void MeshRenderer::Init()
{
	auto MB = MemoryBank::GetInstance();
	ComponentID = typeid(MeshRenderer).hash_code();
	ComponentName = MB->FindComponentName(typeid(MeshRenderer).hash_code());
}

void MeshRenderer::Update()
{}

void MeshRenderer::Release()
{}
