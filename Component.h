#pragma once

struct IComponent
{
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};

class Component : public IComponent
{
protected:
	size_t ComponentID;
	string ComponentName;

};