#include "..\include\directx\d3dx12.h"
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>
using namespace Microsoft;
using namespace Microsoft::WRL;
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")

#define THROW_IF_FAILED(hr) if (FAILED(hr)) { throw COMException(hr, "Error", __FILE__, __LINE__); }
#define USE_PRINTF() TCHAR pBuf[1024] = {}; DWORD dwRead = 0;
#define PRINTF(...)\
	StringCchPrintf(pBuf, 1024, __VA_ARGS__); \
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), pBuf, lstrlen(pBuf), NULL, NULL);

#include <exception>
#include <cstring>
#include <sstream>
#include <strsafe.h>

//异常类
class COMException :public std::exception {
public:
	COMException(HRESULT hr, const std::string& message = "", const std::string& file = "", int line = 0) :m_hrError(hr), m_message(message), m_file(file), m_line(line) {}
	HRESULT Error() const {
		return m_hrError;
	}
	virtual const char* what()const noexcept override {
		return m_message.c_str();
	}
	std::string File()const {
		return m_file;
	}
	int Line()const {
		return m_line;
	}
private:
	const HRESULT m_hrError;
	const std::string m_message;
	const std::string m_file;
	const int m_line;
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	UINT nDXGIFactoryFlags = 0U;
	ComPtr<IDXGIFactory5> pIDXGIFactory5;
	ComPtr<IDXGIAdapter1> pIAdapter1;
	ComPtr<ID3D12Device4> pID3D12Device4;
	USE_PRINTF();
	try {
		AllocConsole();
		//打开显示子系统的调试支持
		{
#if defined(_DEBUG)
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				// 打开附加的调试支持
				nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
#endif
		}
		//创建DXGI Factory对象
		{
			THROW_IF_FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5)));//创建工厂
		}
		//枚举适配器，并选择合适的适配器来创建3D设备对象
		{
			DXGI_ADAPTER_DESC1	desc = {};
			D3D12_FEATURE_DATA_ARCHITECTURE stArchitecture = {};
			for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pIDXGIFactory5->EnumAdapters1(adapterIndex, &pIAdapter1); ++adapterIndex) {
				pIAdapter1->GetDesc1(&desc);
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					continue;//软件虚拟适配器，跳过
				}
				PRINTF(
					_T("显卡[%d]-\"%s\":独占显存[%dMB]、独占内存[%dMB]、共享内存[%dMB] "),
					adapterIndex,
					desc.Description,
					desc.DedicatedVideoMemory / (1024 * 1024),
					desc.DedicatedSystemMemory / (1024 * 1024),
					desc.SharedSystemMemory / (1024 * 1024)
					);
					//创建设备并检查架构
					//D3D12_FEATURE_DATA_ARCHITECTURE.UMA = true一般为集显
					//此时若D3D12_FEATURE_DATA_ARCHITECTURE.CacheCoherentUMA = true 则表示是CC-UMA架构 GPU和CPU通过Cache来交换数据
					//若UMA = FALSE一般表示为独显，此时CacheCoherentUMA无意义
					THROW_IF_FAILED(D3D12CreateDevice(pIAdapter1.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pID3D12Device4)));
					THROW_IF_FAILED(pID3D12Device4->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &stArchitecture, sizeof(stArchitecture)));
					if (stArchitecture.UMA) {
						if (stArchitecture.CacheCoherentUMA) {
							PRINTF(_T("架构为(CC_UMA)"));
						}
						else {
							PRINTF(_T("架构为(UMA)"));
						}
					}
					else {
						PRINTF(_T("架构为(NUMA)"));
					}

					if (stArchitecture.TileBasedRenderer) {
						PRINTF(_T("支持Tile-based方式渲染"));
					}
					pID3D12Device4.Reset();
					PRINTF(_T("\n"));
			}
			::system("pause");
			FreeConsole();
		}
	}
	catch (const COMException& e) {
		std::basic_stringstream<TCHAR> ss;
		ss << _T("Error message: ") << e.what() << std::endl;
		ss << _T("File: ") << (e.File().c_str()) << std::endl;
		ss << _T("Line: ") << e.Line() << std::endl;
		//ss << e.Error();
		MessageBox(NULL, ss.str().c_str(), _T("Error"), MB_OK | MB_ICONERROR);
	}
	return 0;
}
