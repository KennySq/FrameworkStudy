#pragma once

struct Pass
{
	ComPtr<ID3D11InputLayout> IL;
	ComPtr<ID3D11VertexShader> VS;
	ComPtr<ID3D11GeometryShader> GS;
	ComPtr<ID3D11DomainShader> DS;
	ComPtr<ID3D11HullShader> HS;
	ComPtr<ID3D11PixelShader> PS;

	ComPtr<ID3D11ShaderResourceView> RegisterT[8];
	ComPtr<ID3D11SamplerState> RegisterS[8];
	ComPtr<ID3D11UnorderedAccessView> RegisterU[8];
	ComPtr<ID3D11Buffer> RegisterC[8];
};

struct ComputeObject
{
	ComPtr<ID3D11ComputeShader> CS;
	ComPtr<ID3D11UnorderedAccessView> RegisterU[8];

	inline void BindUAV(ID3D11UnorderedAccessView* UAV, UINT Index)
	{
		RegisterU[Index] = UAV;
	}

};