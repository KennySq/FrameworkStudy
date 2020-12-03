#include"stdafx.h"
#include"ModelLoader.h"

// OBJ Loader 1.0
// .obj
HRESULT LoadStaticModelFromFile(string Path, Model* pModel, Material* pMaterial)
{
	Mesh<StaticVertex> MeshInst;
	
	vector<XMFLOAT4> Positions;
	vector<XMFLOAT2> UVs;
	vector<XMFLOAT3> Normals;

	vector<unsigned int> PositionIndices, UVIndices, NormalIndices;



	FILE* file = fopen(Path.c_str(), "r");
	//fstream file = fstream(Path.c_str());
	
	if (!file)
		DebugLog(L_ERROR, Path + " failed to open file");


	auto e = feof(file);
	int res = 0;
	while (res != EOF)
	{
		char Header[128];
		res = fscanf(file, "%s", Header);		

		if (strcmp(Header, "v") == 0)
		{
			XMFLOAT4 Position;
			fscanf(file, "%f %f %f\n", &Position.x, &Position.y, &Position.z);
			
			
			
			Positions.emplace_back(Position);
		}
		else if (strcmp(Header, "vt") == 0)
		{
			XMFLOAT2 UV;
			fscanf(file, "%f %f\n", &UV.x, &UV.y);
			UVs.emplace_back(UV);
		}
		else if (strcmp(Header, "vn") == 0)
		{
			XMFLOAT3 Normal;
			fscanf(file, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z);
			Normals.emplace_back(Normal);
		}
		else if (strcmp(Header, "f") == 0)
		{
			string V1, V2, V3;
			unsigned int VertexIndex[3], UVIndex[3], NormalIndex[3];

			int Matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
								 &VertexIndex[0], &UVIndex[0], &NormalIndex[0],
								 &VertexIndex[1], &UVIndex[1], &NormalIndex[1],
								 &VertexIndex[2], &UVIndex[2], &NormalIndex[2]);

			if (Matches == -1)
				break;

			if (Matches != 9)
			{
				DebugLog(L_ERROR, Path + " This file can't be read in this loader.");
				return E_FAIL;
			}

			PositionIndices.emplace_back(VertexIndex[0]);
			PositionIndices.emplace_back(VertexIndex[1]);
			PositionIndices.emplace_back(VertexIndex[2]);

			UVIndices.emplace_back(UVIndex[0]);
			UVIndices.emplace_back(UVIndex[1]);
			UVIndices.emplace_back(UVIndex[2]);

			NormalIndices.emplace_back(NormalIndex[0]);
			NormalIndices.emplace_back(NormalIndex[1]);
			NormalIndices.emplace_back(NormalIndex[2]);
		}



	}

	for (unsigned int i = 0; i < PositionIndices.size(); i++)
	{
		StaticVertex Vertex;

		unsigned int PositionIndex = PositionIndices[i];
		unsigned int UVIndex = UVIndices[i];
		unsigned int NormalIndex = NormalIndices[i];

		XMFLOAT4 Position = Positions[PositionIndex - 1];
		XMFLOAT2 UV = UVs[UVIndex - 1];
		XMFLOAT3 Normal = Normals[NormalIndex - 1];

		Vertex.Position = Position;
		Vertex.UV = UV;
		Vertex.Normal = Normal;

		MeshInst.Vertices.emplace_back(Vertex);
	}

	for (int i = 0; i < MeshInst.Vertices.size(); i++)
		MeshInst.Indices.emplace_back(i);

	clearerr(file);

	CreateStaticMeshBuffer(MeshInst, pModel);

	MeshInst.Vertices.clear();
	MeshInst.Indices.clear();

	return S_OK;
}

HRESULT CreateStaticMeshBuffer(Mesh<StaticVertex>& const SM, Model* const pModel)
{
	auto Device = D3DHardware::GetInstance().GetDevice();

	HRESULT Result;
	D3D11_BUFFER_DESC Desc{};
	D3D11_SUBRESOURCE_DATA Sub{};

	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Desc.ByteWidth = sizeof(StaticVertex) * SM.Vertices.size();
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Sub.pSysMem = SM.Vertices.data();

	Result = Device->CreateBuffer(&Desc, &Sub, pModel->VertexBuffer.GetAddressOf());
	ResultLog(Result, "Creating vertex buffer");

	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.ByteWidth = sizeof(unsigned int) * SM.Indices.size();
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Sub.pSysMem = SM.Indices.data();

	Result = Device->CreateBuffer(&Desc, &Sub, pModel->IndexBuffer.GetAddressOf());
	ResultLog(Result, "Creating index buffer");

	return Result;
}
