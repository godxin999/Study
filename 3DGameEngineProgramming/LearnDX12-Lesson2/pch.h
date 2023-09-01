#pragma once
#define NOMINMAX //防止Windows.h中的min和max宏定义
#define WIN32_LEAN_AND_MEAN //从Windows头文件中排除极少使用的内容
//DirectX12扩展头文件
#include "..\include\directx\d3dx12.h"
#include <windows.h>
#include <shellapi.h> //For CommandLineToArgvW
//防止宏定义和函数定义冲突
#if defined(CreateWindow)
#undef CreateWindow
#endif
//Windows Runtime Library
#include <wrl.h>
using namespace Microsoft::WRL;
//DirectX12相关头文件
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//DirectX12库文件
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
//STL头文件
#include <algorithm>
#include <cassert>
#include <chrono>
#include <map>

#include "Helpers.h"