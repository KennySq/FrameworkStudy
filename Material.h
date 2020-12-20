#pragma once

HRESULT CompilePassFromFile(string Path, string Entry, Pass* pPass, int CompileFlag);

using namespace std;

// Shader Model 5.0 - 5.1 only
// 1 Pass = 1 hlsl file

struct Material
{
	map<string, Pass*> Passes;
	
	inline void AddPass(string Path, string PassName, int CompileFlag)
	{
		Pass* NewPass = new Pass();
		auto Result = CompilePassFromFile(Path, PassName, NewPass, CompileFlag);

		if (Result != S_OK)
		{
			DebugLog(L_WARNING, (Path + " => Failed to compile pass."));
			return;
		}

		Passes.insert_or_assign(PassName, NewPass);

		return;
	}

};