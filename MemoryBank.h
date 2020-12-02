#pragma once
using namespace std;

class MemoryBank
{
private:
	static shared_ptr<MemoryBank> Instance;

	unordered_map<size_t, string> StringMap;
	unordered_map<size_t, Scene*> SceneMap;
	unordered_map<size_t, string> ComponentMap;

public:
	MemoryBank();
	~MemoryBank();

	template<class _Ty>
	inline void RegisterComponent()
	{
		auto CID = typeid(_Ty).hash_code();
		if (ComponentMap.find(CID) != ComponentMap.end())
		{
			char buffer[128];
			itoa(CID, buffer, 128);
			DebugLog(L_WARNING, string(buffer) + " => is alreay exist.");
			return;
		}

		ComponentMap.insert_or_assign(CID, typeid(_Ty).name());
	}

	inline bool FindComponent(size_t CID)
	{
		if (ComponentMap.find(CID) != ComponentMap.end())
			return true;

		DebugLog(L_WARNING, "There is no such component id.");

		return false;
	}

	inline string FindComponentName(size_t CID)
	{
		if (ComponentMap.find(CID) != ComponentMap.end())
			return ComponentMap[CID];

		DebugLog(L_WARNING, "There is no such component id.");

		return "";
	}

	inline size_t AddScene(Scene* Scen)
	{
		if (!Scen)
		{
			MessageBox(nullptr, L"This scene is not initialized!", 0, 0);
			return;
		}
		
		auto ID = make_hash<Scene*>(Scen);
		SceneMap.insert_or_assign(ID, Scen);

		return ID;
	}

	inline Scene* const GetScene(size_t ID)
	{
		if (SceneMap.find(ID) != SceneMap.end())
			return SceneMap[ID];

		DebugLog(L_WARNING, to_string(ID) + " does not exist.");

		return nullptr;
	}

	inline void AddString(string String)
	{
		auto Key = make_hash<string>(String);

		if(StringMap.find(Key) != StringMap.end())
			StringMap.insert_or_assign(Key, String);
	}

	static MemoryBank* const GetInstance() { if (!Instance) Instance = make_shared<MemoryBank>(); return &*Instance; }
};

