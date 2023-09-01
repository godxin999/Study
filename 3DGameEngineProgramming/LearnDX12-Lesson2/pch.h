#pragma once
#define NOMINMAX //��ֹWindows.h�е�min��max�궨��
#define WIN32_LEAN_AND_MEAN //��Windowsͷ�ļ����ų�����ʹ�õ�����
//DirectX12��չͷ�ļ�
#include "..\include\directx\d3dx12.h"
#include <windows.h>
#include <shellapi.h> //For CommandLineToArgvW
//��ֹ�궨��ͺ��������ͻ
#if defined(CreateWindow)
#undef CreateWindow
#endif
//Windows Runtime Library
#include <wrl.h>
using namespace Microsoft::WRL;
//DirectX12���ͷ�ļ�
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//DirectX12���ļ�
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
//STLͷ�ļ�
#include <algorithm>
#include <cassert>
#include <chrono>
#include <map>

#include "Helpers.h"