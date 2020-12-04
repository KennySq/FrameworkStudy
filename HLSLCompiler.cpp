#include"stdafx.h"
#include"HLSLCompiler.h"

HRESULT ReflectShader(ID3DBlob* VBlob, ID3D11InputLayout** pOutInputLayout, ID3D11ShaderReflection** pOutReflection)
{
	HRESULT Result;

	auto Device = D3DHardware::GetInstance().GetDevice();

	Result = D3DReflect(VBlob->GetBufferPointer(), VBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), (void**)pOutReflection);
	ResultLog(Result, "Reflecting a vertex shader.");

	unsigned int ByteOffset = 0;
	D3D11_SHADER_DESC ShaderDesc{};
	vector<D3D11_INPUT_ELEMENT_DESC> InputElements;
	D3D11_SIGNATURE_PARAMETER_DESC SignParam;
	pOutReflection[0]->GetDesc(&ShaderDesc);
	for (int i = 0; i < ShaderDesc.InputParameters; i++)
	{
		pOutReflection[0]->GetInputParameterDesc(i, &SignParam);

		D3D11_INPUT_ELEMENT_DESC Element;

		Element.SemanticName = SignParam.SemanticName;
		Element.SemanticIndex = SignParam.SemanticIndex;
		Element.InputSlot = 0;
		Element.AlignedByteOffset = ByteOffset;
		Element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Element.InstanceDataStepRate = 0;

		if (SignParam.Mask == 1)
		{
			if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Element.Format = DXGI_FORMAT_R32_UINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Element.Format = DXGI_FORMAT_R32_SINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Element.Format = DXGI_FORMAT_R32_FLOAT;

			ByteOffset += 4;
		}
		else if (SignParam.Mask <= 3)
		{
			if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Element.Format = DXGI_FORMAT_R32G32_UINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Element.Format = DXGI_FORMAT_R32G32_SINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Element.Format = DXGI_FORMAT_R32G32_FLOAT;
			ByteOffset += 8;
		}
		else if (SignParam.Mask <= 7)
		{
			if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Element.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Element.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			ByteOffset += 12;
		}

		else if (SignParam.Mask <= 15)
		{
			if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Element.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Element.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (SignParam.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			ByteOffset += 16;
		}


		InputElements.push_back(Element);
	}

	Result = Device->CreateInputLayout(InputElements.data(), InputElements.size(), VBlob->GetBufferPointer(), VBlob->GetBufferSize(), pOutInputLayout);
	ResultLog(Result, "Creating an InputLayout.");

	return S_OK;
}

HRESULT CompileVS(string Path, string Entry, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppIL)
{
	USES_CONVERSION;

	DWORD Flag = 0;
	auto Device = D3DHardware::GetInstance().GetDevice();
#ifdef DEBUG
	Flag |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* VBlob = nullptr, *ErrBlob = nullptr;
	ID3D11ShaderReflection* pReflection;

	auto Result = D3DCompileFromFile(A2W(Path.c_str()), nullptr, nullptr, (Entry + "VS").c_str(), "vs_5_0", Flag, 0, &VBlob, &ErrBlob);
	ResultLog(Result, Path + " => Compiling HLSL file.");

	Result = Device->CreateVertexShader(VBlob->GetBufferPointer(), VBlob->GetBufferSize(), nullptr, ppVS);
	ResultLog(Result, Path + " => Creating a vertex shader.");

	Result = ReflectShader(VBlob, ppIL, &pReflection);

	return Result;

}

HRESULT CompilePS(string Path, string Entry, ID3D11PixelShader** ppPS)
{
	USES_CONVERSION;

	DWORD Flag = 0;
	auto Device = D3DHardware::GetInstance().GetDevice();
#ifdef DEBUG
	Flag |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* PBlob = nullptr, *ErrBlob = nullptr;
	ID3D11ShaderReflection** ppReflection;

	auto Result = D3DCompileFromFile(A2W(Path.c_str()), nullptr, nullptr, (Entry + "PS").c_str(), "ps_5_0", Flag, 0, &PBlob, &ErrBlob);
	ResultLog(Result, Path + " => Compiling HLSL file.");

	Result = Device->CreatePixelShader(PBlob->GetBufferPointer(), PBlob->GetBufferSize(), nullptr, ppPS);
	ResultLog(Result, Path + " => Creating a pixel shader.");

	return Result;

}

HRESULT CompilePassFromFile(string Path, string Entry, Pass* pPass)
{
	CompileVS(Path, Entry, pPass->VS.GetAddressOf(), pPass->IL.GetAddressOf());
	CompilePS(Path, Entry, pPass->PS.GetAddressOf());

	return S_OK;
}