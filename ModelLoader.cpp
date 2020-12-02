#include"stdafx.h"
#include"ModelLoader.h"

// WIP
// .obj
HRESULT LoadStaticModelFromFile(string Path, Model* pModel, Material* pMaterial)
{
	Mesh<StaticVertex> MeshInst;

	fstream f = fstream(Path.c_str());

	if (!f.good())
		DebugLog(L_ERROR, Path + " failed to open file");

	while (true)
	{
		char Header[128];
		
	}

	return S_OK;
}
