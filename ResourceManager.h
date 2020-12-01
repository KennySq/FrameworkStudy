#pragma once

inline HRESULT GenerateSRVFromRTTexture2D(ID3D11Texture2D* RawTexture, ID3D11ShaderResourceView** ppSRV)
{
	D3D11_TEXTURE2D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};

	RawTexture->GetDesc(&TexInfo);

	SRVDesc.Format = TexInfo.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = TexInfo.MipLevels;
	
	auto Result = Device->CreateShaderResourceView(RawTexture, &SRVDesc, ppSRV);
	ResultLog(Result, "Shader Resource View");

	return Result;
}

inline HRESULT GenerateRTVFromRTTexture2D(ID3D11Texture2D* RawTexture, ID3D11RenderTargetView** ppRTV)
{
	D3D11_TEXTURE2D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc{};

	RawTexture->GetDesc(&TexInfo);

	RTVDesc.Format = TexInfo.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	auto Result = Device->CreateRenderTargetView(RawTexture, &RTVDesc, ppRTV);
	ResultLog(Result, "Render Target View");

	return Result;
}

inline RTTexture2D* const CreateTexture2D(DXGI_FORMAT Format, int Bind, UINT CPUAccess, UINT Width, UINT Height)
{
	auto Device = D3DHardware::GetInstance().GetDevice();
	RTTexture2D* Resource = new RTTexture2D();

	D3D11_TEXTURE2D_DESC TexDesc{};
		
	TexDesc.Format = Format;
	TexDesc.BindFlags = Bind;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.CPUAccessFlags = CPUAccess;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.Width = Width;
	TexDesc.Height= Height;
	TexDesc.MipLevels = 0;
	TexDesc.ArraySize = 1;

	auto Result = Device->CreateTexture2D(&TexDesc, nullptr, Resource->RawTexture.GetAddressOf());
	ResultLog(Result, "RTTexture2D");

	GenerateRTVFromRTTexture2D(Resource->RawTexture.Get(), Resource->RTV.GetAddressOf());
	GenerateSRVFromRTTexture2D(Resource->RawTexture.Get(), Resource->SRV.GetAddressOf());

	return Resource;

}

//---------------------------------------------------------------------------//
// Depth Stencil															 //

inline HRESULT GenerateDSVFromDSTexture2D(ID3D11Texture2D* RawTexture, ID3D11DepthStencilView** ppDSV)
{
	D3D11_TEXTURE2D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};

	RawTexture->GetDesc(&TexInfo);
	
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	auto Result = Device->CreateDepthStencilView(RawTexture, &DSVDesc, ppDSV);
	ResultLog(Result, "Depth Stencil View");
	
	return Result;
}

inline DSTexture2D* CreateDepthStencil(UINT Width, UINT Height)
{
	auto Device = D3DHardware::GetInstance().GetDevice();
	DSTexture2D* Resource = new DSTexture2D();

	D3D11_TEXTURE2D_DESC TexDesc{};

	TexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.Width = Width;
	TexDesc.Height = Height;
	TexDesc.MipLevels = 0;
	TexDesc.ArraySize = 1;

	auto Result = Device->CreateTexture2D(&TexDesc, nullptr, Resource->RawTexture.GetAddressOf());
	ResultLog(Result, "RTTexture2D");

	GenerateDSVFromDSTexture2D(Resource->RawTexture.Get(), Resource->DSV.GetAddressOf());

	return Resource;
}