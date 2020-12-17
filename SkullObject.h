#pragma once
#include "Instance.h"
class SkullObject :
	public Instance
{
private:
	MeshRenderer* MR;
	RTTexture2D Tex;
public:

	virtual void Init() override;
	virtual void Update(float Delta) override;

	SkullObject();
	~SkullObject();
};

