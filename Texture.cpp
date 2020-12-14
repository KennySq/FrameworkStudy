#include"stdafx.h"
#include"Texture.h"

void LoadCubemapFromDDS(const char * Path, RTTexture2D * const RT)
{
	USES_CONVERSION;

	auto Device = D3DHardware::GetInstance().GetDevice();
	auto Result = CreateDDSTextureFromFile(Device, A2W(Path),
				(ID3D11Resource**)RT->RawTexture.GetAddressOf(),
				RT->SRV.GetAddressOf());

	ResultLog(Result, "Creating DDS texture from file.");

}
