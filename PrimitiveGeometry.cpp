#include"stdafx.h"
#include"PrimitiveGeometry.h"

HRESULT GenerateSphere(float radius, UINT sliceCount, UINT stackCount, Model * pModel)
{
	Mesh<StaticVertex> M;

	StaticVertex topVertex(0.0f,+radius, 0.0f, 1.0f ,0.0f, 1.0f, 0.0f ,0.0f, 0.0f);
	StaticVertex bottomVertex(0.0f, -radius, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,0.0f, 1.0f);

	M.Vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			StaticVertex v;

			// spherical to cartesian
			v.Position.x = radius * sinf(phi)*cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi)*sinf(theta);

			//// Partial derivative of P with respect to theta
			//v.TangentU.x = -radius * sinf(phi)*sinf(theta);
			//v.TangentU.y = 0.0f;
			//v.TangentU.z = +radius * sinf(phi)*cosf(theta);

			//XMVECTOR T = XMLoadFloat3(&v.TangentU);
			//XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat4(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.UV.x = theta / XM_2PI;
			v.UV.y = phi / XM_PI;

			M.Vertices.push_back(v);
		}
	}

	M.Vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		M.Indices.push_back(0);
		M.Indices.push_back(i + 1);
		M.Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			M.Indices.push_back(baseIndex + i * ringVertexCount + j);
			M.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			M.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			M.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			M.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			M.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)M.Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		M.Indices.push_back(southPoleIndex);
		M.Indices.push_back(baseIndex + i);
		M.Indices.push_back(baseIndex + i + 1);
	}

	auto Result = CreateStaticMeshBuffer(M, pModel);
	ResultLog(Result, "Creating Sphere Mesh.");

	return Result;
}

HRESULT GenerateQuad(Model* pModel)
{
	Mesh<StaticVertex> m;

	StaticVertex V[] = {
		StaticVertex(-1.0f, -1.0f, 0.0f, 1.0f,0.0f,0.0f,0.0f, 0.0f, 1.0f),
		StaticVertex(1.0f, -1.0f, 0.0f, 1.0f,0.0f,0.0f,0.0f, 1.0f, 1.0f),
		StaticVertex(-1.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f,0.0f, 0.0f, 0.0f),
		StaticVertex(1.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f,0.0f, 1.0f, 0.0f),
	};

	UINT I[] = { 0, 1, 2, 2,1,3 };

	m.Vertices.emplace_back(V[0]);
	m.Vertices.emplace_back(V[1]);
	m.Vertices.emplace_back(V[2]);
	m.Vertices.emplace_back(V[3]);

	m.Indices.emplace_back(I[0]);
	m.Indices.emplace_back(I[1]);
	m.Indices.emplace_back(I[2]);
	m.Indices.emplace_back(I[3]);
	m.Indices.emplace_back(I[4]);
	m.Indices.emplace_back(I[5]);

	auto Result = CreateStaticMeshBuffer(m, pModel);
	ResultLog(Result, "Creating Quad Mesh.");

	return Result;
}
