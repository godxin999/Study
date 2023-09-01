#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Game.h"
#include "CommandQueue.h"

constexpr wchar_t WINDOW_CLASS_NAME[] = L"DX12RenderwndClass";

using WindowPtr = std::shared_ptr<Window>;
using WindowMap = std::map<HWND, WindowPtr>;
using WindowNameMap = std::map<std::wstring, WindowPtr>;

static Application* gs_pSingelton = nullptr;
static WindowMap gs_Windows;
static WindowNameMap gs_WindowByName;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return 0; }

struct MakeWindow :public Window {
	MakeWindow(HWND hWnd, const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync) :
		Window(hWnd, windowName, clientWidth, clientHeight, vSync) {}
};

Application::Application(HINSTANCE hInst) :m_hInstance(hInst), m_TearingSupported(false) {
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);//设置线程的DPI感知上下文

#if defined(_DEBUG)
	//在创建ID3D12Device之前启用调试层，否则将会导致运行时删除设备
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif

	WNDCLASSEXW wndClass = {};

	wndClass.cbSize = sizeof(WNDCLASSEXW);//结构体大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//窗口样式，设置为当横纵方向有变化时重绘
	wndClass.lpfnWndProc = &WndProc;//窗口回调函数
	wndClass.cbClsExtra = 0;//窗口类的附加内存，一般为0
	wndClass.cbWndExtra = 0;//窗口的附加内存，一般为0
	wndClass.hInstance = hInst;//应用实例句柄
	wndClass.hIcon = ::LoadIcon(hInst, nullptr);//图标句柄，可以使用LoadIcon函数从资源文件加载图标，值为nullptr则使用默认图标
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);//光标句柄，这里使用默认光标
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//背景画刷句柄，颜色值必须是标准系统颜色之一，且必须加1
	wndClass.lpszMenuName = nullptr;//指向菜单名称的字符串，这里不使用菜单，所以为nullptr
	wndClass.lpszClassName = WINDOW_CLASS_NAME;//指向窗口类名的字符串
	wndClass.hIconSm = ::LoadIcon(hInst, nullptr);//与窗口类关联的小图标句柄

	if (!RegisterClassExW(&wndClass)) {
		MessageBoxW(nullptr, L"Unable to register the window class.", L"Error", MB_OK | MB_ICONERROR);
	}

	m_dxgiAdapter = GetAdapter(false);

	if (m_dxgiAdapter) {
		m_d3d12Device = CreateDevice(m_dxgiAdapter);
	}
	if (m_d3d12Device) {
		m_DirectCommandQueue = std::make_shared<CommandQueue>(m_d3d12Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
		m_ComputeCommandQueue = std::make_shared<CommandQueue>(m_d3d12Device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
		m_CopyCommandQueue = std::make_shared<CommandQueue>(m_d3d12Device, D3D12_COMMAND_LIST_TYPE_COPY);
		m_TearingSupported = CheckTearingSupport();
	}
}

void Application::Create(HINSTANCE hInst) {
	if (!gs_pSingelton) {
		gs_pSingelton = new Application(hInst);
	}
}

Application& Application::Get() {
	assert(gs_pSingelton);
	return *gs_pSingelton;
}

void Application::Destory() {
	if (gs_pSingelton) {
		assert(gs_Windows.empty() && gs_WindowByName.empty() && "All windows should be destroyed before destroying the application instance.");
		delete gs_pSingelton;
		gs_pSingelton = nullptr;
	}
}

Application::~Application() {
	Flush();
}

Microsoft::WRL::ComPtr<IDXGIAdapter4> Application::GetAdapter(bool useWarp) {
	ComPtr<IDXGIFactory4> dxgiFactory;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	ComPtr<IDXGIAdapter1> dxgiAdapter1;
	ComPtr<IDXGIAdapter4> dxgiAdapter4;

	if (useWarp) {
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
		ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
	}
	else {
		SIZE_T maxDedicatedVideoMemory = 0;
		for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_NONE) == 0 && //跳过软件虚拟适配器
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) && //检查适配器对D3D的支持级别，返回接口的参数被值为nullptr，这样就不会创建设备，只是检查支持级别
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory) { //优先选择独占显存最大的适配器
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}
	return dxgiAdapter4;
}

Microsoft::WRL::ComPtr<ID3D12Device2> Application::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter) {
	ComPtr<ID3D12Device2> d3d12Device2;
	ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(d3d12Device2.As(&pInfoQueue))) {
		//当调试信息严重到以下级别时，程序将中断
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);//内存损坏时生成
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
	}
	//虽然在发布应用程序前应解决所有警告，但是解决所有可能发生的警告并不现实，所以可以使用过滤器来忽略一些警告消息
	//按类别进行忽略
	//D3D12_MESSAGE_CATEGORY Categories[] = {};
	//按消息严重性级别来忽略消息
	D3D12_MESSAGE_SEVERITY Severities[] = {
		D3D12_MESSAGE_SEVERITY_INFO
	};
	//通过ID忽略单个消息
	D3D12_MESSAGE_ID DenyIds[] = {
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,//当使用不是在资源创建期间指定的优化透明颜色的透明颜色清除渲染目标时，会出现此警告
		D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE, //在图形调试时使用捕获帧时会出现此警告
		D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE //在图形调试时使用捕获帧时会出现此警告
	};

	D3D12_INFO_QUEUE_FILTER NewFilter = {};
	//NewFilter.DenyList.NumCategories = _countof(Categories);
	//NewFilter.DenyList.pCategoryList = Categories;
	NewFilter.DenyList.NumSeverities = _countof(Severities);
	NewFilter.DenyList.pSeverityList = Severities;
	NewFilter.DenyList.NumIDs = _countof(DenyIds);
	NewFilter.DenyList.pIDList = DenyIds;

	ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));//将过滤器推送到消息队列中
#endif
	return d3d12Device2;
}

bool Application::CheckTearingSupport() {
	BOOL allowTearing = FALSE;
	//图形调试工具不支持DXGI1.5，所以先创建DXGI1.4接口，然后使用As获取DXGI1.5接口
	ComPtr<IDXGIFactory4> factory4;//IDXGIFactory4不包含CheckFeatureSupport方法
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4)))) {
		ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5))) {
			if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing)))) {
				allowTearing = FALSE;
			}
		}
	}
	return allowTearing == TRUE;
}

bool Application::IsTearingSupported()const {
	return m_TearingSupported;
}


std::shared_ptr<Window> Application::CreateRenderWindow(const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync) {
	auto windowIter = gs_WindowByName.find(windowName);
	if (windowIter != gs_WindowByName.end()) {
		return windowIter->second;
	}

	RECT windowRect = { 0,0,clientWidth,clientHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hWnd = CreateWindowW(WINDOW_CLASS_NAME, windowName.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, m_hInstance, nullptr
	);

	if (!hWnd) {
		MessageBoxW(nullptr, L"Could not create the render window.", L"Error", MB_OK | MB_ICONERROR);
		return nullptr;
	}

	WindowPtr pWindow = std::make_shared<MakeWindow>(hWnd, windowName, clientWidth, clientHeight, vSync);

	gs_Windows.insert(WindowMap::value_type(hWnd, pWindow));
	gs_WindowByName.insert(WindowNameMap::value_type(windowName, pWindow));

	return pWindow;
}

void Application::DestroyWindow(std::shared_ptr<Window> window) {
	if (window) {
		window->Destory();
	}
}

void Application::DestroyWindow(const std::wstring& windowName) {
	WindowPtr pWindow = GetWindowByName(windowName);
	if (pWindow) {
		DestroyWindow(pWindow);
	}
}

std::shared_ptr<Window> Application::GetWindowByName(const std::wstring& windowName) {
	WindowPtr pWindow;
	auto it = gs_WindowByName.find(windowName);
	if (it != gs_WindowByName.end()) {
		pWindow = it->second;
	}
	return pWindow;
}

int Application::Run(std::shared_ptr<Game> pGame) {
	if (!pGame->Initialize())return 1;
	if (!pGame->LoadContent())return 2;

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	Flush();

	pGame->UnloadContent();
	pGame->Destroy();

	return static_cast<int>(msg.wParam);
}

void Application::Quit(int exitCode) {
	::PostQuitMessage(exitCode);
}

Microsoft::WRL::ComPtr<ID3D12Device2> Application::GetDevice()const {
	return m_d3d12Device;
}

std::shared_ptr<CommandQueue> Application::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) const {
	std::shared_ptr<CommandQueue> commandQueue;
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		commandQueue = m_DirectCommandQueue;
		break;
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		commandQueue = m_ComputeCommandQueue;
		break;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		commandQueue = m_CopyCommandQueue;
		break;
	default:
		assert(false && "Invalid command queue type.");
	}
	return commandQueue;
}

void Application::Flush() {
	m_DirectCommandQueue->Flush();
	m_ComputeCommandQueue->Flush();
	m_CopyCommandQueue->Flush();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Application::CreateDescriptorHeap(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type) {
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(m_d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

UINT Application::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const {
	return m_d3d12Device->GetDescriptorHandleIncrementSize(type);
}

