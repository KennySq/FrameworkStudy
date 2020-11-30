#include"stdafx.h"
#include"Hardware.h"

shared_ptr<D3DHardware> D3DHardware::Instance = nullptr;

HRESULT D3DHardware::GenerateDevice()
{
	HRESULT Result;
	D3D_FEATURE_LEVEL Level[] =
	{
		D3D_FEATURE_LEVEL_11_1,
	};

	DXGI_SWAP_CHAIN_DESC SwapChainDesc{};

	DWORD Flag = 0;
#ifdef _DEBUG
	Flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	SwapChainDesc.Flags = Flag;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Width = 800;
	SwapChainDesc.BufferDesc.Height = 600;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.OutputWindow = WindowHandle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Windowed = true;

	Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
							   nullptr, Flag, nullptr, 0, D3D11_SDK_VERSION, &SwapChainDesc, SwapChain.GetAddressOf(), 
							   Device.GetAddressOf(), Level, Context.GetAddressOf());

	ResultLog(Result, "D3D Device and SwapChain.");

	if (Result != S_OK)
	{
		MessageBox(nullptr, L"Failed to create d3d device.", 0, 0);
		return E_FAIL;
	}

	return S_OK;
}
