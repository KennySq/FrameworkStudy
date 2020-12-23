#pragma once

using namespace Microsoft::WRL;
using namespace std;

struct RTTexture2D
{
	ComPtr<ID3D11Texture2D> RawTexture;
	ComPtr<ID3D11RenderTargetView> RTV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

struct RTTexture2DArray
{
	ComPtr<ID3D11Texture2D> RawTexture;
	vector<ComPtr<ID3D11RenderTargetView>> RTV;
	vector<ComPtr<ID3D11ShaderResourceView>> SRV;
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
	ComPtr<ID3D11RenderTargetView> RTV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};

struct DSTexture2D
{
	ComPtr<ID3D11Texture2D> RawTexture;
	ComPtr<ID3D11DepthStencilView> DSV;
	ComPtr<ID3D11ShaderResourceView> SRV;
};
struct DSTexture2DArray
{
	ComPtr<ID3D11Texture2D> RawTexture;
	vector<ComPtr<ID3D11DepthStencilView>> DSV;
	vector<ComPtr<ID3D11ShaderResourceView>> SRV;
};


void LoadCubemapFromDDS(const char* Path, RTTexture2D* const RT);