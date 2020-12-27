#pragma once

class Component;

enum RENDER_TYPE
{
	FORWARD,
	DEFERRED,
};

struct MeshRenderer : public Component
{
	ImmediateRenderer* IR = nullptr;
	Model* RenderModel = nullptr; // None
	std::vector<Material*> Materials;

	Pass* CurrentPass = nullptr;

	Pass* ForwardPass = nullptr;
	Pass* DeferredPass = nullptr;
	Material* DeferredMat = nullptr;
	Material* ForwardMat = nullptr;

	RENDER_TYPE RenderType = RENDER_TYPE::DEFERRED;

	vector<ID3D11RasterizerState*> RasterizerStates;

	inline void SetRenderType(int Type) { RenderType = (RENDER_TYPE)Type; }

	inline void UnbindUAV()
	{
		static ID3D11UnorderedAccessView* UAV[] = { nullptr };
		static auto Context = D3DHardware::GetInstance().GetContext();

		Context->CSSetUnorderedAccessViews(0, 1, UAV, nullptr);
	}

	inline void BindUAV(ID3D11UnorderedAccessView* UAV, UINT Index)
	{
		if (!UAV)
		{
			DebugLog(L_WARNING, "The UAV is empty.");
			return;
		}

		CurrentPass->RegisterU[Index] = UAV;
	}

	inline void UnbindSRV()
	{
		static ID3D11ShaderResourceView* SRV[] = { nullptr };
		static auto Context = D3DHardware::GetInstance().GetContext();

		Context->PSSetShaderResources(0, 1, SRV);
	}

	inline void BindSRV(ID3D11ShaderResourceView* SRV, UINT Index)
	{

		if (!SRV)
		{
			DebugLog(L_WARNING, "The SRV is empty.");
			return;
		}

		static auto Context = D3DHardware::GetInstance().GetContext();

		CurrentPass->RegisterT[Index] = SRV;
		Context->PSSetShaderResources(Index, 1, &SRV);
	}

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

	void Forward() = delete;
	void Deferred() = delete;
	void Screen() = delete;

	MeshRenderer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;
};