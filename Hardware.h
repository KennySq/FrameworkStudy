#pragma once

using namespace std;
using namespace Microsoft::WRL;
struct D3DHardware
{
private:
	static shared_ptr<D3DHardware> Instance;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;

	shared_ptr<InputManager> Input;

	HWND WindowHandle;
	HINSTANCE WindowInstance;

	UINT WindowWidth;
	UINT WindowHeight;

	D3D11_RASTERIZER_DESC RasterizerDesc{};

	HRESULT GenerateDevice();

public:
	static D3DHardware& GetInstance(HWND hWnd = nullptr, HINSTANCE hInstance = nullptr) { 
		if (!Instance)
		{
			Instance = make_shared<D3DHardware>();
			Instance->WindowHandle = hWnd;
			Instance->WindowInstance = hInstance;

			Instance->RasterizerDesc = CD3D11_RASTERIZER_DESC();
			Instance->RasterizerDesc.FillMode = D3D11_FILL_SOLID;
			Instance->RasterizerDesc.CullMode = D3D11_CULL_BACK;
			Instance->RasterizerDesc.DepthClipEnable = true;

			Instance->GenerateDevice();

		}

		return *Instance;
	}

	auto GetRSDesc() { return RasterizerDesc;  }

	auto GetDevice() { return Device.Get(); }
	auto GetContext() { return Context.Get(); }
	auto GetSwapChain() { return SwapChain.Get(); }

	auto const GetInputManager() { return Input.get(); }

	auto GetDeviceAddress() { return Device.GetAddressOf(); }
	auto GetContextAddress() { return Context.GetAddressOf(); }
};