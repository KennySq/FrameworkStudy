#pragma once
using namespace std;

class Scene;
class MemoryBank
{
private:
	static shared_ptr<MemoryBank> Instance;

	unordered_map<size_t, string> StringMap;
	unordered_map<size_t, Scene*> SceneMap;
	unordered_map<size_t, string> ComponentMap;
	unordered_map<size_t, ComputeObject*> ComputeCache;


	vector<Material*> MaterialCache;

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
			return NULL;
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

	inline void AddMaterial(Material* Mat)
	{
		if (!Mat)
		{
			DebugLog(L_ERROR, "This material pointer was invalid");
			return;
		}
		MaterialCache.emplace_back(Mat);
	}

	inline void AssignComputeObject(string Path, string PassName)
	{
		ComputeObject* CO = new ComputeObject();

		auto Result = CompileComputeShaderFromFile(Path, PassName, CO);
		ResultLog(Result, "Compile Compute Shader.");

		ComputeCache.insert_or_assign(make_hash(PassName), CO);
	}

	inline void AssignMaterialPass(string Path, string PassName, int CompileFlag)
	{
		Material* Mat = new Material();

		Mat->AddPass(Path, PassName,CompileFlag);

		MaterialCache.emplace_back(Mat);
	}

	inline Material* const GetMaterial(UINT Index)
	{
		if (Index >= MaterialCache.size() && Index < 0)
		{
			DebugLog(L_ERROR, "Index out of range.");
			return nullptr;
		}

		return MaterialCache[Index];
	}

	inline Material* const GetMaterialByPass(const char* PassName)
	{
		for (auto m : MaterialCache)
		{
			if (m->Passes.find(PassName) != m->Passes.end())
				return m;
		}

		return nullptr;
	}

	inline ComputeObject* const GetComputeObjectByPass(const char* PassName)
	{
		for (auto c : ComputeCache)
		{
			if (make_hash(string(PassName)) == c.first)
				return c.second;
		}

		return nullptr;
	}

	static MemoryBank* const GetInstance() { if (!Instance) Instance = make_shared<MemoryBank>(); return &*Instance; }
};