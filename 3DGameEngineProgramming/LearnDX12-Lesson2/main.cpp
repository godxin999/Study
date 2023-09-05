#include "pch.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include "Application.h"
#include "Cube.h"

#include <dxgidebug.h>


void ReportLiveObjects() {
	IDXGIDebug1* dxgiDebug;
	ThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));//��ȡ���Խӿ�

	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);//����Direct3D�л�������Ϣ����һ��������ʾ�����������͵Ķ��󣬵ڶ���������ʾ�����ڲ�����
	dxgiDebug->Release();//�ͷŽӿ�ָ��
}

//����ڵ�
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
	
	atexit(&ReportLiveObjects);//ע����ֹ�������������˳����ִ��ReportLiveObjects����

	return retCode;
}











