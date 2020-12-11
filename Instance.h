#pragma once

using namespace std;
class Scene;
class Instance
{
private:
	string Name;
	size_t IID; // Generated by it's own pointer (hash)

	Scene* RootScene;
	unordered_map<size_t, Component*> Components;

public:
	inline unordered_map<size_t, Component*>& const GetAllComponents() { return Components; }

	inline void SetScene(Scene* const pScene) { RootScene = pScene; }
	inline Scene* const GetScene() { return RootScene; }

	inline void SetName(string& Str) { Name = Str; }
	inline void SetName(const char* Str) { Name = Str; }

	inline string& const GetName() { return Name; }
	inline size_t const GetIID() { return IID; }

	template<class _Ty>
	inline _Ty* AddComponent()
	{
		auto Memory = MemoryBank::GetInstance();
		if (!Memory->FindComponent(typeid(_Ty).hash_code()))
		{
			DebugLog(L_WARNING, "Cannot find component");
			return nullptr;
		}

		_Ty* Comp = new _Ty();

		auto CompPtr = ((Component*)&*Comp);

		Components.insert_or_assign(typeid(_Ty).hash_code(), CompPtr);
		CompPtr->Root = this;
		CompPtr->Init();

		return Comp;
	}

	template<class _Ty>
	inline _Ty* const GetComponent()
	{
		auto CID = typeid(_Ty).hash_code();
		if (Components.find(CID) != Components.end())
			return (_Ty*)Components[CID];

		DebugLog(L_ERROR, typeid(_Ty).name() + string(" does not exist."));

		return nullptr;
	}

	template<class _Ty>
	inline void RemoveComponent()
	{
		if (!Components.find(typeid(_Ty).hash_code()))
		{
			DebugLog(L_WARNING, "This instance doesn't have this component.");
			return;
		}

		Components.erase(typeid(_Ty).hash_code());

		return;
	}

	virtual void Init();
	virtual void Update(float Delta);
	virtual void Render(float Delta);
	virtual void Release();

	Instance();
	~Instance();
};

