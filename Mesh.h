#pragma once

struct Vertex
{
	XMFLOAT4 Position;
};

struct StaticVertex : Vertex
{
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
};

struct SkinnedVertex : StaticVertex
{
	
};

template<class _VertTy>
struct Mesh
{
	vector<_VertTy> Vertices;
	vector<UINT> Indices;
};