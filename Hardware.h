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

	HWND WindowHandle;

	HRESULT GenerateDevice();

public:
	static D3DHardware& GetInstance(HWND hWnd = nullptr) { 
		if (!Instance)
		{
			Instance = make_shared<D3DHardware>();
			Instance->WindowHandle = hWnd;
			Instance->GenerateDevice();
		}

		return *Instance;
	}

	auto GetDevice() { return Device.Get(); }
	auto GetContext() { return Context.Get(); }
	auto GetSwapChain() { return SwapChain.Get(); }

	auto GetDeviceAddress() { return Device.GetAddressOf(); }
	auto GetContextAddress() { return Context.GetAddressOf(); }
};