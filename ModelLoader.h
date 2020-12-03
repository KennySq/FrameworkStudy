#pragma once

// .Obj
HRESULT LoadStaticModelFromFile(string Path, Model* pModel, Material* pMaterial = nullptr);
HRESULT CreateStaticMeshBuffer(Mesh<StaticVertex>& const SM, Model* const pModel);
