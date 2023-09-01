//DirectX12扩展头文件
#include "..\include\directx\d3dx12.h"
//#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN //从Windows头文件中排除极少使用的内容
#include <windows.h>
#include <shellapi.h> //For CommandLineToArgvW
//防止宏定义和函数定义冲突
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif
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

#include "Helpers.h"
//定义控制应用初始化的变量
const uint8_t g_NumFrames = 3;
bool g_UseWarp = false;
uint32_t g_ClientWidth = 1280;
uint32_t g_ClientHeight = 720;
bool g_IsInitialized = false;
//Windows相关变量
HWND g_HWnd;
RECT g_WindowRect;
//DirectX相关变量
ComPtr<ID3D12Device2> g_Device;
ComPtr<ID3D12CommandQueue> g_CommandQueue;
ComPtr<IDXGISwapChain4> g_SwapChain;
ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];
ComPtr<ID3D12GraphicsCommandList> g_CommandList;
ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];
ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
UINT g_RTVDescriptorSize;
UINT g_CurrentBackBufferIndex;
//用于同步的变量
ComPtr<ID3D12Fence> g_Fence;
uint64_t g_FenceValue = 0;
uint64_t g_FrameFenceValues[g_NumFrames];
HANDLE g_FenceEvent;
//默认启用垂直同步
bool g_VSync = true;//控制交换链的Present方法是否应等待下一次垂直刷新(垂直同步)
bool g_TearingSupported = false;//默认不支持屏幕撕裂
//默认使用窗口模式，可使用Alt+Enter/F11切换
bool g_Fullsceen = false;
//窗口回调函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//解析命令函参数
void ParseCommandLineArguments() {
	int argc;
	wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
	for (auto i = 0; i < argc; ++i) {
		//指定渲染窗口宽度
		if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0) {
			g_ClientWidth = ::wcstoul(argv[++i], nullptr, 10);
		}
		//指定渲染窗口高度
		if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0) {
			g_ClientHeight = ::wcstoul(argv[++i], nullptr, 10);
		}
		//指定是否使用WARP进行设备创建
		if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) {
			g_UseWarp = true;
		}
		//可扩展以处理其他命令行参数
	}
	::LocalFree(argv);//释放CommandLineToArgvW分配的内存
}

//在调试版本中启用调试层
void EnableDebugLayer() {
#if defined(_DEBUG)
	//在创建ID3D12Device之前启用调试层，否则将会导致运行时删除设备
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif
}

//注册窗口
void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName) {
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEXW);//结构体大小
	windowClass.style = CS_HREDRAW | CS_VREDRAW;//窗口样式，设置为当横纵方向有变化时重绘
	windowClass.lpfnWndProc = &WndProc;//窗口回调函数
	windowClass.cbClsExtra = 0;//窗口类的附加内存，一般为0
	windowClass.cbWndExtra = 0;//窗口的附加内存，一般为0
	windowClass.hInstance = hInst;//应用实例句柄
	windowClass.hIcon = ::LoadIcon(hInst, nullptr);//图标句柄，可以使用LoadIcon函数从资源文件加载图标，值为nullptr则使用默认图标
	windowClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);//光标句柄，这里使用默认光标
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//背景画刷句柄，颜色值必须是标准系统颜色之一，且必须加1
	windowClass.lpszMenuName = nullptr;//指向菜单名称的字符串，这里不使用菜单，所以为nullptr
	windowClass.lpszClassName = windowClassName;//指向窗口类名的字符串
	windowClass.hIconSm = ::LoadIcon(hInst, nullptr);//与窗口类关联的小图标句柄

	static ATOM atom = ::RegisterClassExW(&windowClass);
	assert(atom > 0);//atom值大于0则注册成功，否则注册失败抛出异常
}

//创建窗口
HWND CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height) {
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);//获取屏幕宽度
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);//获取屏幕高度

	RECT windowRect = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);//根据窗口样式调整窗口矩形

	int windowWidth = windowRect.right - windowRect.left;//调整后的窗口宽度
	int windowHeight = windowRect.bottom - windowRect.top;//调整后的窗口高度

	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);//窗口左上角x坐标
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);//窗口左上角y坐标

	HWND hWnd = ::CreateWindowExW(
		NULL,//扩展窗口样式
		windowClassName,//窗口类名
		windowTitle,//窗口标题
		WS_OVERLAPPEDWINDOW,//窗口样式
		windowX,//窗口初始水平位置
		windowY,//窗口初始垂直位置
		windowWidth,//窗口宽度
		windowHeight,//窗口高度
		nullptr,//父窗口句柄
		nullptr,//菜单句柄
		hInst,//应用实例句柄
		nullptr//指向窗口创建数据的指针
	);

	assert(hWnd && "Failed to create window");

	return hWnd;
}

//查询DirectX12适配器
ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp) {
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

//创建DirectX12设备
ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter) {
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

//创建命令队列
ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12CommandQueue> d3d12CommandQueue;
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;//单GPU时为0

	ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
	return d3d12CommandQueue;
}

//撕裂支持检测
bool CheckTearingSupport() {
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

//创建交换链
ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, uint32_t width, uint32_t height, uint32_t bufferCount) {
	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory4> dxgiFactory4;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;//是否是立体声，如果是立体声还要指定flip-mode交换链
	swapChainDesc.SampleDesc = { 1,0 };//当使用flip-mode交换链时，必须指定为{1,0}
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//用于渲染目标输出
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;//指示DXGI将后台缓冲区的内容进行缩放以适应演示目标的大小
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//设置为翻转模式，并且不保留之前呈现的帧
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;//标识交换链后台缓冲区的透明度行为，这里是未指定
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;//设置为支持屏幕撕裂

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(//用于创建与操作系统窗口句柄关联的交换链
		commandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,//不使用全屏模式，设置为nullptr
		nullptr,//不限制到特定的输出，设置为nullptr
		&swapChain1//只接受IDXGISwapChain1接口
	));
	//禁止ALT+ENTER全屏切换
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));//手动处理以获得无边框窗口

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	return dxgiSwapChain4;
}

//创建描述符堆
ComPtr<ID3D12DescriptorHeap> CreateDescritorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors) {
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;

	ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

//创建RTV
void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap) {
	auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < g_NumFrames; ++i) {
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));//获取指向交换链后台缓冲区的指针
		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);//nullptr代表创建默认描述符

		g_BackBuffers[i] = backBuffer;

		rtvHandle.Offset(rtvDescriptorSize);//偏移到下一个描`述符
	}
}

//创建命令分配器
ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
	return commandAllocator;
}

//创建命令列表
ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
	ThrowIfFailed(commandList->Close());
	return commandList;
}

//创建栅栏
ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device) {
	ComPtr<ID3D12Fence> fence;
	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	return fence;
}

//创建操作系统事件
HANDLE CreateEventHandle() {
	HANDLE fenceEvent;
	fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);//创建一个自动重置的初始状态为无信号的事件
	assert(fenceEvent && "Failed to create fence event.");
	return fenceEvent;
}

//Signal信号函数
uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue) {
	//该函数从GPU向栅栏发出信号，只有在GPU命令队列在执行过程中达到该点时才会发出信号
	uint64_t fenceValueForSignal = ++fenceValue;
	ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));
	return fenceValueForSignal;
}

//Wait等待函数
void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::microseconds duration = std::chrono::milliseconds::max()) {
	if (fence->GetCompletedValue() < fenceValue) {
		ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));//指定达到fenceValue时，触发fenceEvent事件
		::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));//阻塞线程等待事件触发或者超时
	}
}

//Flush刷新函数
void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent) {
	//该函数用于确保之前在GPU上执行的任何命令都已执行完毕，该函数返回后可以安全地释放资源
	uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
	WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}

//Update更新函数
void Update() {
	static uint64_t frameCounter = 0;
	static double elapsedSeconds = 0.0;
	static std::chrono::high_resolution_clock clock;//时钟变量为high_resolution_clock，采样时间点为std::chrono::time_point
	static auto t0 = clock.now();

	++frameCounter;
	auto t1 = clock.now();//默认单位为nanosecond
	auto deltaTime = t1 - t0;
	t0 = t1;

	elapsedSeconds += deltaTime.count() * 1e-9;
	if (elapsedSeconds > 1.0) {
		wchar_t buffer[500];
		auto fps = frameCounter / elapsedSeconds;
		swprintf_s(buffer, 500, L"FPS: %f\n", fps);
		OutputDebugString(buffer);
		frameCounter = 0;
		elapsedSeconds = 0.0;
	}
}

//Render渲染函数
void Render() {
	auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];
	//重置命令分配器和命令列表
	commandAllocator->Reset();
	g_CommandList->Reset(commandAllocator.Get(), nullptr);
	//Clear清除
	{
		//在清除渲染目标前，要先将其转换为RENDER_TARGET状态
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		g_CommandList->ResourceBarrier(1, &barrier);//插入资源屏障

		FLOAT clearColor[] = { 0.4f,0.6f,0.9f,1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), g_CurrentBackBufferIndex, g_RTVDescriptorSize);//获取当前帧的RTV描述符句柄
		g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);//nullptr表示清除整个资源视图
	}
	//Present呈现
	{
		//在呈现前，要先将其转换为PRESENT状态
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);
		g_CommandList->ResourceBarrier(1, &barrier);
		ThrowIfFailed(g_CommandList->Close());
		ID3D12CommandList* const commandLists[] = {
			g_CommandList.Get()
		};
		g_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
		UINT syncInterval = g_VSync ? 1 : 0;
		UINT presentFlags = g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		ThrowIfFailed(g_SwapChain->Present(syncInterval, presentFlags));
		//Signal返回的栅栏值被用于阻塞CPU线程，直到资源使用完毕
		g_FrameFenceValues[g_CurrentBackBufferIndex] = Signal(g_CommandQueue, g_Fence, g_FenceValue);//更新当前帧的栅栏值
		g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();//在Present之后后台缓冲区的索引可能改变
		WaitForFenceValue(g_Fence, g_FrameFenceValues[g_CurrentBackBufferIndex], g_FenceEvent);
	}
}

//Resize重置大小函数
void Resize(uint32_t width, uint32_t height) {
	if (g_ClientWidth != width || g_ClientHeight != height) {
		g_ClientHeight = std::max(1u, width);
		g_ClientWidth = std::max(1u, height);
		//在调整交换链大小时，需要释放对交换链后台缓冲区的引用，由于在GPU上可能有一个运行中的命令列表引用了交换链的后台缓冲区，所以需要先刷新GPU
		Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
		for (int i = 0; i < g_NumFrames; ++i) {
			g_BackBuffers[i].Reset();//重置后台缓冲区
			g_FrameFenceValues[i] = g_FrameFenceValues[g_CurrentBackBufferIndex];//将栅栏值设置为当前帧的栅栏值
		}
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		ThrowIfFailed(g_SwapChain->GetDesc(&swapChainDesc));
		ThrowIfFailed(g_SwapChain->ResizeBuffers(g_NumFrames, g_ClientWidth, g_ClientHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
		//在调整交换链大小后，后台缓冲区的索引可能改变，所以重新进行获取，并创建新的RTV
		g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
		UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
	}
}

//全屏切换函数
void SetFullScreen(bool fullscreen) {
	if (g_Fullsceen != fullscreen) {
		g_Fullsceen = fullscreen;
		if (g_Fullsceen) {
			::GetWindowRect(g_HWnd, &g_WindowRect);
			//将窗口样式设置为无边框窗口
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			::SetWindowLongW(g_HWnd, GWL_STYLE, windowStyle);//GWL_STYLE表示设置窗口样式

			HMONITOR hMoniter = ::MonitorFromWindow(g_HWnd, MONITOR_DEFAULTTONEAREST);//检索距离应用程序窗口最近的显示器的句柄
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMoniter, &monitorInfo);//获取显示器的信息
			::SetWindowPos(g_HWnd, HWND_TOP,//HWND_TOP表示将窗口置于Z序的顶部
				monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				//SWP_FRAMECHANGED标志在使用SetWindowLong函数更改窗口的框架样式时，可以使用此标志来应用新的框架样式。
				//它会向窗口发送一个WM_NCCALCSIZE消息，即使窗口的大小没有改变也会发送此消息。
				//如果没有指定此标志，则只有在窗口大小改变时才会发送WM_NCCALCSIZE消息
				//SWP_NOACTIVATE标志用于在调用SetWindowPos函数时不激活窗口。
				//如果没有设置此标志，窗口将被激活并移动到最顶层或非最顶层组的顶部(具体取决于hWndInsertAfter参数的设置)
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(g_HWnd, SW_MAXIMIZE);//最大化窗口
		}
		else {
			::SetWindowLong(g_HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//更改回原来窗口的样式
			::SetWindowPos(g_HWnd, HWND_NOTOPMOST,
				g_WindowRect.left,
				g_WindowRect.top,
				g_WindowRect.right - g_WindowRect.left,
				g_WindowRect.bottom - g_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(g_HWnd, SW_NORMAL);//激活并显示窗口
		}
	}
}

//窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (g_IsInitialized) {
		switch (message)
		{
		case WM_PAINT:
		{
			Update();
			Render();
			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
			switch (wParam)
			{
			case 'V':
				g_VSync = !g_VSync;
				break;
			case VK_ESCAPE:
				::PostQuitMessage(0);
				break;
			case VK_RETURN:
				if (alt) {
			case VK_F11:
				SetFullScreen(!g_Fullsceen);
				}
				break;
			}
			break;
		}
		case WM_SYSCHAR:
			//如果未处理WM_SYSCHAR消息，则默认窗口过程将处理该消息，并在窗口有键盘焦点时播放系统通知音
			break;
		case WM_SIZE:
		{
			RECT clientRect = {};
			::GetClientRect(g_HWnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			Resize(width, height);
			break;
		}
		case WM_DESTROY:
			//如果不响应此消息，可能导致应用实例无法退出
			::PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProcW(hWnd, message, wParam, lParam);//使用默认窗口过程处理消息
		}
	}
	else {
		return ::DefWindowProcW(hWnd, message, wParam, lParam);//使用默认窗口过程处理消息
	}
	return 0;
}

//主入口点
int CALLBACK wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
	//Windows 10 Creators 更新添加了 Per Monitor V2 DPI 感知上下文
	//使用此感知上下文允许窗口的客户端区域实现 100% 缩放，同时仍然允许以 DPI 敏感方式呈现非客户端窗口内容
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);//设置线程的DPI感知上下文
	const wchar_t* windowClassName = L"DX12WindowClass";//窗口类名
	ParseCommandLineArguments();//解析命令行参数
	EnableDebugLayer();//启用调试层
	g_TearingSupported = CheckTearingSupport();//检查撕裂支持
	RegisterWindowClass(hinstance, windowClassName);//注册窗口类
	g_HWnd = CreateWindow(windowClassName, hinstance, L"Learn Dx12", g_ClientWidth, g_ClientHeight);//创建窗口
	::GetWindowRect(g_HWnd, &g_WindowRect);//获取窗口矩形

	ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(g_UseWarp);

	g_Device = CreateDevice(dxgiAdapter4);

	g_CommandQueue = CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

	g_SwapChain = CreateSwapChain(g_HWnd, g_CommandQueue, g_ClientWidth, g_ClientHeight, g_NumFrames);

	g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();

	g_RTVDescriptorHeap = CreateDescritorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);

	g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);

	for (int i = 0; i < g_NumFrames; ++i) {
		g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	g_CommandList = CreateCommandList(g_Device, g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

	g_Fence = CreateFence(g_Device);
	g_FenceEvent = CreateEventHandle();

	g_IsInitialized = true;

	::ShowWindow(g_HWnd, SW_SHOW);

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);

	::CloseHandle(g_FenceEvent);

	return 0;
}











