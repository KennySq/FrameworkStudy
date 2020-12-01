#pragma once

class Component;
struct MeshRenderer : public Component
{
	Model* RenderModel = nullptr; // None
	vector<Material*> Materials;

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

	virtual void Init() override;
	virtual void Update() override;
	virtual void Release() override;
};