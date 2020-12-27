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
	ResultLog(Result, "Shader Resource View (2D)");

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
	ResultLog(Result, "Render Target View (2D)");

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

inline HRESULT GenerateRTVFromTexture3D(ID3D11Texture3D* Resource, ID3D11RenderTargetView** ppRTV)
{
	D3D11_TEXTURE3D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc{};

	Resource->GetDesc(&TexInfo);

	RTVDesc.Format = TexInfo.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;

	auto Result = Device->CreateRenderTargetView(Resource, &RTVDesc, ppRTV);
	ResultLog(Result, "Render Target View (3D)");

	return Result;
}

inline HRESULT GenerateUAVFromTexture3D(ID3D11Texture3D* Resource, ID3D11UnorderedAccessView** ppUAV)
{
	D3D11_TEXTURE3D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc{};

	Resource->GetDesc(&TexInfo);

	UAVDesc.Format = TexInfo.Format;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
	UAVDesc.Texture3D.WSize = TexInfo.Depth;
	auto Result = Device->CreateUnorderedAccessView(Resource, &UAVDesc, ppUAV);
	ResultLog(Result, "Unordered Access View (3D)");

	return Result;
}

inline HRESULT GenerateSRVFromTexture3D(ID3D11Texture3D* Resource, ID3D11ShaderResourceView** ppSRV)
{
	D3D11_TEXTURE3D_DESC TexInfo{};
	auto Device = D3DHardware::GetInstance().GetDevice();
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};

	Resource->GetDesc(&TexInfo);

	SRVDesc.Format = TexInfo.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	SRVDesc.Texture3D.MipLevels = 1;
	auto Result = Device->CreateShaderResourceView(Resource, &SRVDesc, ppSRV);
	ResultLog(Result, "Shader Resource View (3D)");
	
	return Result;
}

inline HRESULT CreateTexture3D(DXGI_FORMAT Format, int Bind, UINT Width, UINT Height, UINT Depth, RTTexture3D* const pTex)
{
	auto Device = D3DHardware::GetInstance().GetDevice();
	
	D3D11_TEXTURE3D_DESC TexDesc{};

	TexDesc.Format = Format;
	TexDesc.BindFlags = Bind;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.Width = Width;
	TexDesc.Height = Height;
	TexDesc.Depth = Depth;
	TexDesc.MipLevels = 0;
	TexDesc.MiscFlags = 0;

	auto Result = Device->CreateTexture3D(&TexDesc, nullptr, pTex->RawTexture.GetAddressOf());
	ResultLog(Result, "RTTexture3D");

	Result = GenerateRTVFromTexture3D(pTex->RawTexture.Get(), pTex->RTV.GetAddressOf());
	Result = GenerateSRVFromTexture3D(pTex->RawTexture.Get(), pTex->SRV.GetAddressOf());

	return Result;
	
}

inline HRESULT CreateUnorderedAccessTexture3D(DXGI_FORMAT Format, int Bind, UINT Width, UINT Height, UINT Depth, UATexture3D* const pTex)
{
	auto Device = D3DHardware::GetInstance().GetDevice();

	D3D11_TEXTURE3D_DESC TexDesc{};

	TexDesc.Format = Format;
	TexDesc.BindFlags = Bind;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.Width = Width;
	TexDesc.Height = Height;
	TexDesc.Depth = Depth;
	TexDesc.MipLevels = 1;
	TexDesc.MiscFlags = 0;

	auto Result = Device->CreateTexture3D(&TexDesc, nullptr, pTex->RawTexture.GetAddressOf());
	ResultLog(Result, "UATexture3D");

	Result = GenerateUAVFromTexture3D(pTex->RawTexture.Get(), pTex->UAV.GetAddressOf());
	//Result = GenerateRTVFromTexture3D(pTex->RawTexture.Get(), pTex->RTV.GetAddressOf());
	Result = GenerateSRVFromTexture3D(pTex->RawTexture.Get(), pTex->SRV.GetAddressOf());

	return Result;

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