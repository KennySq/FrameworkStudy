#include"stdafx.h"
#include"Scene.h"

Scene::Scene()
{
	auto Device = D3DHardware::GetInstance().GetDevice();

	D3D11_BUFFER_DESC LBDesc{};

	LBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LBDesc.ByteWidth = sizeof(SceneLight);
	LBDesc.Usage = D3D11_USAGE_DEFAULT;

	auto Result = Device->CreateBuffer(&LBDesc, nullptr, LightBuffer.GetAddressOf());
	ResultLog(Result, "Creating light buffer of " + SceneName);
}

Scene::~Scene()
{

}

void Scene::UpdateLights()
{
	static auto Context = D3DHardware::GetInstance().GetContext();

	Context->UpdateSubresource(LightBuffer.Get(), 0, nullptr, &LightInfo, 0, 0);
}

void Scene::AddDirectionalLight(XMVECTOR Direction, float Intensity)
{
	DirectionalLight Light;

	XMStoreFloat3(&Light.Direction, Direction);
	Light.Intensity = Intensity;

	DirectionalLights.emplace_back(Light);

	LightInfo.DirectionalLights[LightInfo.DirectionalCount] = DirectionalLights[LightInfo.DirectionalCount];

	LightInfo.DirectionalCount++;
}

void Scene::AddSpotLight(XMVECTOR Position, float Radius)
{
	SpotLight Light;

	XMStoreFloat3(&Light.Position, Position);
	Light.Radius = Radius;

	SpotLights.emplace_back(Light);

	LightInfo.SpotLights[LightInfo.SpotCount] = SpotLights[LightInfo.SpotCount];

	LightInfo.SpotCount++;

}

void Scene::Init()
{
	for (auto i : Instances)
		i.second->Init();

	return;
}

void Scene::Update(float Delta)
{
	UpdateLights();

	for (auto i : Instances)
	{
		auto Comps = i.second->GetAllComponents();
	
		i.second->Update(Delta);

		for (auto c : Comps)
		{
			c.second->Update();
		}

		i.second->Render(Delta);

	}

}
