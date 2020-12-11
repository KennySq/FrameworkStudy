#pragma once
#include "Instance.h"
class SkullObject :
	public Instance
{
public:

	virtual void Init() override;
	virtual void Update(float Delta) override;

	SkullObject();
	~SkullObject();
};

