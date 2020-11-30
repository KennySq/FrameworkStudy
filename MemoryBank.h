#pragma once
using namespace std;

class MemoryBank
{
private:
	static shared_ptr<MemoryBank> Instance;

	unordered_map<size_t, string> StringMap;
	unordered_map<size_t, Scene*> SceneMap;

public:
	MemoryBank();
	~MemoryBank();

	inline void AddScene(Scene* Scen)
	{
		if (!Scen)
		{
			MessageBox(nullptr, L"This scene is not initialized!", 0, 0);
			return;
		}
		

		SceneMap.insert_or_assign(make_hash<Scene*>(Scen), Scen);
	}

	inline void AddString(string String)
	{
		auto Key = make_hash<string>(String);

		if(StringMap.find(Key) != StringMap.end())
			StringMap.insert_or_assign(Key, String);
	}

	static MemoryBank& GetInstance() { if (!Instance) Instance = make_shared<MemoryBank>(); return *Instance; }
};

