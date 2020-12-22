#include"stdafx.h"
#include"MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
	auto MB = MemoryBank::GetInstance();
	RenderModel = new Model();
	IR = &ImmediateRenderer::GetInstance();

	DeferredMat = MB->GetMaterialByPass("Deferred");
	DeferredPass = DeferredMat->Passes["Deferred"];
}

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
	static auto GBuffer = IR->GetGBuffer();

	static auto TransformComp = Root->GetComponent<Transform>();
	static auto TRSBuffer = TransformComp->GetBuffer();
	static auto LightBuffer = RootScene->GetLightBuffer();
	static auto CameraBuffer = Camera->GetBuffer();

	static UINT Strides[] = { sizeof(StaticVertex) };
	static UINT Offsets[] = { 0 };

	static D3D11_BUFFER_DESC IndexDesc{};
	static UINT IndexCount;
	static D3D11_TEXTURE2D_DESC GBufferDesc;

	GBuffer->RawTexture.Get()->GetDesc(&GBufferDesc);
	ID3D11Buffer* ConstantBuffers[] = {  TRSBuffer, CameraBuffer.Get(), LightBuffer };

	IR->SetRenderTarget(&GBuffer, 1, IR->GetDepthStencils()[1]);

	Context->VSSetShader(DeferredPass->VS.Get(), nullptr, 0);
	Context->PSSetShader(DeferredPass->PS.Get(), nullptr, 0);
	Context->VSSetConstantBuffers(0, 3, ConstantBuffers);
	Context->PSSetConstantBuffers(0, 3, ConstantBuffers);
	Context->IASetInputLayout(CurrentPass->IL.Get());


	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetVertexBuffers(0, 1, RenderModel->VertexBuffer.GetAddressOf(), Strides, Offsets);
	Context->IASetIndexBuffer(RenderModel->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Context->RSSetState(RasterizerStates[0]);

	RenderModel->IndexBuffer.Get()->GetDesc(&IndexDesc);
	
	Context->DrawIndexed(IndexDesc.ByteWidth / sizeof(unsigned int), 0, 0);

	IR->UnsetRenderTarget();
}

void MeshRenderer::Release()
{}
