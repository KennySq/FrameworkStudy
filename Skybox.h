#pragma once
#include "Instance.h"
class Skybox :
	public Instance
{
	MeshRenderer* MR;
	Transform* TRS;
	
	RTTexture2D Tex;
	
public:

	virtual void Init() override;
	virtual void Update(float Delta) override;
	virtual void Render(float Delta) override;

	Skybox();
	~Skybox();
};

