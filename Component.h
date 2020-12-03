#pragma once

struct IComponent
{
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

class Instance;
class Component : public IComponent
{
protected:
	Instance* Root;

	size_t ComponentID;
	string ComponentName;

};