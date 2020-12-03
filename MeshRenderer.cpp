#include"stdafx.h"
#include"MeshRenderer.h"

void MeshRenderer::Init()
{
	auto MB = MemoryBank::GetInstance();
	ComponentID = typeid(MeshRenderer).hash_code();
	ComponentName = MB->FindComponentName(typeid(MeshRenderer).hash_code());
}

void MeshRenderer::Update()
{

	static auto Context = D3DHardware::GetInstance().GetContext();
	static auto RootScene = Root->GetScene();
	static auto Camera = RootScene->GetMainCamera();

	ID3D11Buffer* ConstantBuffers[] = { Camera->GetBuffer().Get() };

	//for(int i=0;i<)
	Context->VSSetShader(Materials[0]->Passes[0]->VS.Get(), nullptr, 0);
	Context->PSSetShader(Materials[0]->Passes[0]->PS.Get(), nullptr, 0);
	Context->VSSetConstantBuffers()
}

void MeshRenderer::Release()
{}
