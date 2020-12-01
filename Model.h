#pragma once

#include"Mesh.h"

struct Model
{
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;
};

struct StaticModel : Model
{
	vector<shared_ptr<Mesh<StaticVertex>>> Meshes;
	//vector<shared_ptr<Material>> Materials;
};