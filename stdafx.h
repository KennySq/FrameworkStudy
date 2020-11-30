#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.

#include<atlbase.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include<string>
#include<iostream>

#include<map>
#include<vector>
#include<unordered_map>

#include<wrl.h>

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include<d3d11.h>
#include<dxgi.h>
#include<d3dcompiler.h>

#include<DirectXMath.h>
#include<DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Utility
#include"Utility.h"

// Hardware
#include"Hardware.h"

// Engine primal resources
#include"Texture.h"

#include"ResourceManager.h"

// Interfaces
#include"IRenderer.h"

#include"ImmediateRenderer.h"

// Engine Resources
#include"Instance.h"
#include"Scene.h"

#include"MemoryBank.h"

#include"Application.h"