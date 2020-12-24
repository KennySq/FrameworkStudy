#pragma once
using namespace DirectX;
struct Vertex
{
	XMFLOAT4 Position;
};

struct StaticVertex : public Vertex
{
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
	
	StaticVertex() { }
	StaticVertex(float px, float py, float pz, float pw, float nx, float ny, float nz, float u, float v)
		: Normal(nx,ny,nz), UV(u,v)
	{
		Position = { px,py,pz,pw };
	}
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