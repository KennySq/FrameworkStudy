#pragma once

using namespace std;

class Scene
{
private:
	unordered_map<size_t, Instance*> Instances;
	
public:
	inline size_t AddInstance(Instance* Inst) { 
		if (!Inst)
		{
			MessageBox(nullptr, L"This instance is not initialized!", 0, 0);
			return;
		}

		auto IID = make_hash<Instance*>(Inst);

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

};