#include "stdafx.h"
#include "Application.h"

shared_ptr<Application> Application::Inst = nullptr;

bool Application::Init()
{
	Hardware = &D3DHardware::GetInstance(WindowHandle);
	Renderer = &ImmediateRenderer::GetInstance();
	auto Memory = MemoryBank::GetInstance();

	Memory->RegisterComponent<MeshRenderer>();
	Memory->RegisterComponent<Camera>();

	if (!Hardware || !Renderer)
		AssertCritical("Hardware or Renderer didn't initialized!", E_INVALIDARG);

	// Swapchain Buffer
	Renderer->AddTexture2D(Renderer->GetBufferFromSwapChain());

	// Render Target #0
	Renderer->AddTexture2D(CreateTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, Width, Height));
	Renderer->AddDepthTexture(CreateDepthStencil(Width, Height));

	// Clear Swapchain buffer
	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Aqua);

	Renderer->AddViewport(CD3D11_VIEWPORT(0.0f, 0.0f, Width, Height));
	auto SceneID = Memory->AddScene(new Scene());
	SelectedScene = Memory->GetScene(SceneID);

	auto InstanceID = SelectedScene->AddInstance(new Instance());
	auto CameraID = SelectedScene->AddInstance(new Instance());

	Instance* SelectedInst = SelectedScene->FindInstanceWithIID(InstanceID);
	Instance* CameraInst = SelectedScene->FindInstanceWithIID(CameraID);

	SelectedInst->AddComponent<MeshRenderer>();
	CameraInst->AddComponent<Camera>();
	
	auto MR = SelectedInst->GetComponent<MeshRenderer>();
	
	Model* M = new Model();
	
	LoadStaticModelFromFile("Assets/Skull/SkullV.obj", M);

	SelectedScene->AddSpotLight(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	MR->SetModel(M);

	return true;
}

void Application::Update()
{
	SelectedScene->Update(0.0f);
}

void Application::Render()
{
	Renderer->SwapFrame();
}

void Application::Release()
{

}
