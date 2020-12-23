#include "stdafx.h"
#include "Application.h"

shared_ptr<Application> Application::Inst = nullptr;

void Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	ImGui::StyleColorsDark();


}

bool Application::Init()
{
	Hardware = &D3DHardware::GetInstance(WindowHandle, WindowInstance);
	Renderer = &ImmediateRenderer::GetInstance();
	Input = D3DHardware::GetInstance().GetInputManager();

	auto Memory = MemoryBank::GetInstance();

	Memory->RegisterComponent<MeshRenderer>();
	Memory->RegisterComponent<Camera>();
	Memory->RegisterComponent<Transform>();

	Memory->AssignMaterialPass("Assets/Shaders/Deferred.hlsl", "Deferred", FLAG_VS | FLAG_PS);
	Memory->AssignMaterialPass("Assets/Shaders/Deferred.hlsl", "Quad", FLAG_VS | FLAG_PS);
	Memory->AssignMaterialPass("Assets/Shaders/SkullObject.hlsl", "SkullObject", FLAG_PS);
	//Memory->AssignComputeObject("Assets/Shaders/VolumeTexture.hlsl", "VolumeTexture");

	if (!Hardware || !Renderer)
		AssertCritical("Hardware or Renderer didn't initialized!", E_INVALIDARG);

	Renderer->GenerateGBuffers();

	// Swapchain Buffer
	Renderer->AddTexture2D(Renderer->GetBufferFromSwapChain());

	// Render Target #0
	Renderer->AddTexture2D(CreateTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, Width, Height));
	Renderer->AddDepthTexture(CreateDepthStencil(Width, Height));

	// Clear Swapchain buffer
	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Aqua);
	Renderer->ClearDepthStencil(Renderer->GetDepthStencils()[0]);

	Renderer->ClearGBuffer(Colors::Black);
	Renderer->ClearDepthStencil(Renderer->GetGDepth());

	Renderer->SetRenderTarget(Renderer->GetTextures2D(), 1, Renderer->GetDepthStencils()[0]);

	Renderer->AddViewport(CD3D11_VIEWPORT(0.0f, 0.0f, Width, Height));
	auto SceneID = Memory->AddScene(new Scene());
	SelectedScene = Memory->GetScene(SceneID);
	SelectedScene->SetName("Testing Scene");

	auto CameraID = SelectedScene->AddInstance(new Instance());

	Instance* CameraInst = SelectedScene->FindInstanceWithIID(CameraID);

	CameraInst->AddComponent<Camera>();

	SelectedScene->AddInstance(new SkullObject());
	//SelectedScene->AddInstance(new RenderableQuad());
//	SelectedScene->AddInstance(new Skybox());

	SelectedScene->AddSpotLight(XMVectorSet(20.0f, 30.0f, 0.0f, 1.0f), 25.0f);
	//SelectedScene->AddSpotLight(XMVectorSet(0.0f, 10.0f, 0.0f, 1.0f), 10.0f);


	SelectedScene->AddDirectionalLight(
		XMVector3Normalize(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f) - XMVectorSet(30.0f, 30.0f, 30.0f, 1.0f)), 1.0f);

	SelectedScene->Init();

	InitImGui();
	ImGui_ImplDX11_Init(Hardware->GetDevice(), Hardware->GetContext());

	ImGui_ImplWin32_Init(WindowHandle);

	auto MainCam = SelectedScene->GetMainCamera();
	auto ForwardBind = [this, MainCam]() { CameraForward(MainCam); };
	auto BackwardBind = [this, MainCam]() { CameraBackward(MainCam); };
	auto LeftBind = [this, MainCam]() { CameraLeft(MainCam); };
	auto RightBind = [this, MainCam]() { CameraRight(MainCam); };
	auto UpBind = [this, MainCam]() { CameraUp(MainCam); };
	auto DownBind = [this, MainCam]() { CameraDown(MainCam); };
	
	auto PitchBind = [this, MainCam](int dx, int dy) {CameraRotateOrbitPitch(MainCam,dy ); };
	auto YawBind = [this, MainCam](int dx, int dy) {CameraRotateOrbitYaw(MainCam,dx); };

	Input->AddTask(DIK_W, ForwardBind);
	Input->AddTask(DIK_S, BackwardBind);
	Input->AddTask(DIK_A, LeftBind);
	Input->AddTask(DIK_D, RightBind);
	Input->AddTask(DIK_Q, UpBind);
	Input->AddTask(DIK_E, DownBind);

//	Input->AddMouseTask(0, PitchBind);
//	Input->AddMouseTask(0, YawBind);

	AppTimer.Start();

	return true;
}

void Application::Update()
{
	static auto Context = D3DHardware::GetInstance().GetContext();

	AppTimer.Stop();
	Renderer->SetRenderTarget(Renderer->GetTextures2D(), 1, nullptr);
	Context->RSSetViewports(1, &Renderer->GetViewport(0));

	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Black);
	Renderer->ClearDepthStencil(Renderer->GetDepthStencils()[0]);

	Renderer->ClearGBuffer(Colors::Black);
	Renderer->ClearDepthStencil(Renderer->GetGDepth());


	Input->Frame();
	
	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui_SceneViewer(SelectedScene);
	
	

	ImGui::EndFrame();
	

	ImGui::Render();
	
	SelectedScene->Update(AppTimer.GetDelta());
}

void Application::Render()
{


	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	Renderer->SwapFrame();

}

void Application::Release()
{
	
}
