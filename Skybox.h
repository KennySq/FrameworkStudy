#pragma once
#include "Instance.h"
class Skybox :
	public Instance
{

public:

	virtual void Init() override;
	virtual void Update(float Delta) override;

	Skybox();
	~Skybox();
};

