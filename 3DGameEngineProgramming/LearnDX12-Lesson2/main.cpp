#include "pch.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include "Application.h"
#include "Cube.h"

#include <dxgidebug.h>


void ReportLiveObjects() {
	IDXGIDebug1* dxgiDebug;
	ThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));//获取调试接口

	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);//报告Direct3D中活动对象的信息，第一个参数表示报告所有类型的对象，第二个参数表示忽略内部对象
	dxgiDebug->Release();//释放接口指针
}

//主入口点
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {

	int retCode = 0;

	WCHAR path[MAX_PATH];
	HMODULE hModule = GetModuleHandleW(nullptr);
	if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0) {
		PathRemoveFileSpecW(path);
		SetCurrentDirectoryW(path);
	}

	Application::Create(hInstance);
	{
		std::shared_ptr<Cube> cube = std::make_shared<Cube>(L"LearnDX12-Lesson2", 1280, 720);
		retCode = Application::Get().Run(cube);
	}
	Application::Destroy();
	
	atexit(&ReportLiveObjects);//注册终止处理函数，程序退出后会执行ReportLiveObjects函数

	return retCode;
}











