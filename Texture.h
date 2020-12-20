#pragma once

using namespace Microsoft::WRL;

struct RTTexture2D
{
	ComPtr<ID3D11Texture2D> RawTexture;
	ComPtr<ID3D11RenderTargetView> RTV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

struct RTTexture3D
{
	ComPtr<ID3D11Texture3D> RawTexture;
	ComPtr<ID3D11RenderTargetView> RTV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

struct UATexture3D
{
	ComPtr<ID3D11Texture3D> RawTexture;
	ComPtr<ID3D11UnorderedAccessView> UAV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

struct DSTexture2D
{
	ComPtr<ID3D11Texture2D> RawTexture;
	ComPtr<ID3D11DepthStencilView> DSV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

void LoadCubemapFromDDS(const char* Path, RTTexture2D* const RT);