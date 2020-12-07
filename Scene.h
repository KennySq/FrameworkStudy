#pragma once

using namespace std;

struct SceneLight
{
	DirectionalLight DirectionalLights[1024];
	SpotLight SpotLights[1024];

	unsigned int DirectionalCount = 0;
	unsigned int SpotCount = 0;
	unsigned int Padding;
	unsigned int Padding2;
};

class Scene
{
private:
	string SceneName;

	unordered_map<size_t, Instance*> Instances;

	vector<DirectionalLight> DirectionalLights;
	vector<SpotLight> SpotLights;

	ComPtr<ID3D11Buffer> LightBuffer;
	SceneLight LightInfo;

	Camera* MainCamera = nullptr;

public:
	Scene();
	~Scene();

	inline size_t AddInstance(Instance* Inst) { 
		if (!Inst)
		{
			MessageBox(nullptr, L"This instance is not initialized!", 0, 0);
			return NULL;
		}

		auto IID = make_hash<Instance*>(Inst);

		Inst->SetScene(this);
		Instances.insert_or_assign(IID, Inst);
		
		return IID;
	}

	inline Instance* const FindInstanceWithIID(size_t IID)
	{
		if(Instances.find(IID) != Instances.end())
			return Instances[IID];

		DebugLog(L_WARNING, to_string(IID) + " does not exist.");
	
		return nullptr;
	}

	void UpdateLights();

	void AddDirectionalLight(XMVECTOR Direction, float Intensity = 1.0f);
	void AddSpotLight(XMVECTOR Position, float Radius = 1.0f);

	inline void SetMainCamera(Camera* const Cam) { MainCamera = Cam; }
	inline Camera* const GetMainCamera() { return MainCamera; }

	inline ID3D11Buffer* const GetLightBuffer() { return LightBuffer.Get(); }

	void Update(float Delta);

};