#pragma once

enum COMPILE_FLAG
{
	FLAG_VS = 1,
	FLAG_PS = 2,
	FLAG_GS = 4,
	FLAG_DS = 8,
	FLAG_HS = 16,
	FLAG_CS = 32,
};

HRESULT ReflectShader(ID3DBlob* VBlob, ID3D11InputLayout** pOutInputLayout, ID3D11ShaderReflection** pOutReflection);
HRESULT CompileVS(string Path, string Entry, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppIL);
HRESULT CompilePS(string Path, string Entry, ID3D11PixelShader** ppPS);
HRESULT CompilePassFromFile(string Path, string Entry, Pass* pPass, int CompileFlag);