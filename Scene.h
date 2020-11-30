#pragma once

using namespace std;

class Scene
{
private:
	vector<Instance*> Instances;
	
public:
	inline void AddInstance(Instance* Inst) { 
		if (!Inst)
		{
			MessageBox(nullptr, L"This instance is not initialized!", 0, 0);
			return;
		}
		Instances.emplace_back(Inst);
	}
};