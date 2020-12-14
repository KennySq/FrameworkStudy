#pragma once

class Component;
struct MeshRenderer : public Component
{

	Model* RenderModel = nullptr; // None
	std::vector<Material*> Materials;

	Pass* CurrentPass = nullptr;

	vector<ID3D11RasterizerState*> RasterizerStates;


	inline void SetModel(Model* const Source)
	{
		if (!Source)
		{
			DebugLog(L_WARNING, "The source is empty.");
			return;
		}

		RenderModel = Source;
		return;
	}


	inline void SetPass(const char* PassName)
	{
		if (Materials[0]->Passes.find(PassName) != Materials[0]->Passes.cend())
			CurrentPass = Materials[0]->Passes[PassName];
	}

	inline void AddRS(D3D11_RASTERIZER_DESC& RSDesc)
	{
		ID3D11RasterizerState** RS = new ID3D11RasterizerState*();

		auto Device = D3DHardware::GetInstance().GetDevice();

		auto Result = Device->CreateRasterizerState(&RSDesc, RS);
		ResultLog(Result, "Creating Rasterizer State.");

		RasterizerStates.emplace_back(*RS);

	}

	inline void AddRS(ID3D11RasterizerState* const RS)
	{
		if (!RS)
		{
			DebugLog(L_ERROR, "RS was empty.");
			return;
		}

		RasterizerStates.emplace_back(RS);
	}

	MeshRenderer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;
};