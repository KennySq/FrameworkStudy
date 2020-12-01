#include "stdafx.h"
#include "Application.h"

shared_ptr<Application> Application::Inst = nullptr;

bool Application::Init()
{
	Hardware = &D3DHardware::GetInstance(WindowHandle);
	Renderer = &ImmediateRenderer::GetInstance();
	auto Memory = MemoryBank::GetInstance();

	Memory->RegisterComponent<MeshRenderer>();

	if (!Hardware || !Renderer)
		AssertCritical("Hardware or Renderer didn't initialized!", E_INVALIDARG);

	// Swapchain Buffer
	Renderer->AddTexture2D(Renderer->GetBufferFromSwapChain());

	// Render Target #0
	Renderer->AddTexture2D(CreateTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, Width, Height));
	Renderer->AddDepthTexture(CreateDepthStencil(Width, Height));

	// Clear Swapchain buffer
	Renderer->ClearTexture(Renderer->GetTextures2D()[0], Colors::Aqua);

	return true;
}

void Application::Update()
{
	
}

void Application::Render()
{
	Renderer->SwapFrame();
}

void Application::Release()
{

}
