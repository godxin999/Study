//DirectX12��չͷ�ļ�
#include "..\include\directx\d3dx12.h"
//#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN //��Windowsͷ�ļ����ų�����ʹ�õ�����
#include <windows.h>
#include <shellapi.h> //For CommandLineToArgvW
//��ֹ�궨��ͺ��������ͻ
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

#include "Helpers.h"
//�������Ӧ�ó�ʼ���ı���
const uint8_t g_NumFrames = 3;
bool g_UseWarp = false;
uint32_t g_ClientWidth = 1280;
uint32_t g_ClientHeight = 720;
bool g_IsInitialized = false;
//Windows��ر���
HWND g_HWnd;
RECT g_WindowRect;
//DirectX��ر���
ComPtr<ID3D12Device2> g_Device;
ComPtr<ID3D12CommandQueue> g_CommandQueue;
ComPtr<IDXGISwapChain4> g_SwapChain;
ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];
ComPtr<ID3D12GraphicsCommandList> g_CommandList;
ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];
ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
UINT g_RTVDescriptorSize;
UINT g_CurrentBackBufferIndex;
//����ͬ���ı���
ComPtr<ID3D12Fence> g_Fence;
uint64_t g_FenceValue = 0;
uint64_t g_FrameFenceValues[g_NumFrames];
HANDLE g_FenceEvent;
//Ĭ�����ô�ֱͬ��
bool g_VSync = true;//���ƽ�������Present�����Ƿ�Ӧ�ȴ���һ�δ�ֱˢ��(��ֱͬ��)
bool g_TearingSupported = false;//Ĭ�ϲ�֧����Ļ˺��
//Ĭ��ʹ�ô���ģʽ����ʹ��Alt+Enter/F11�л�
bool g_Fullsceen = false;
//���ڻص�����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//�����������
void ParseCommandLineArguments() {
	int argc;
	wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
	for (auto i = 0; i < argc; ++i) {
		//ָ����Ⱦ���ڿ��
		if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0) {
			g_ClientWidth = ::wcstoul(argv[++i], nullptr, 10);
		}
		//ָ����Ⱦ���ڸ߶�
		if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0) {
			g_ClientHeight = ::wcstoul(argv[++i], nullptr, 10);
		}
		//ָ���Ƿ�ʹ��WARP�����豸����
		if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) {
			g_UseWarp = true;
		}
		//����չ�Դ������������в���
	}
	::LocalFree(argv);//�ͷ�CommandLineToArgvW������ڴ�
}

//�ڵ��԰汾�����õ��Բ�
void EnableDebugLayer() {
#if defined(_DEBUG)
	//�ڴ���ID3D12Device֮ǰ���õ��Բ㣬���򽫻ᵼ������ʱɾ���豸
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif
}

//ע�ᴰ��
void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName) {
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEXW);//�ṹ���С
	windowClass.style = CS_HREDRAW | CS_VREDRAW;//������ʽ������Ϊ�����ݷ����б仯ʱ�ػ�
	windowClass.lpfnWndProc = &WndProc;//���ڻص�����
	windowClass.cbClsExtra = 0;//������ĸ����ڴ棬һ��Ϊ0
	windowClass.cbWndExtra = 0;//���ڵĸ����ڴ棬һ��Ϊ0
	windowClass.hInstance = hInst;//Ӧ��ʵ�����
	windowClass.hIcon = ::LoadIcon(hInst, nullptr);//ͼ����������ʹ��LoadIcon��������Դ�ļ�����ͼ�ֵ꣬Ϊnullptr��ʹ��Ĭ��ͼ��
	windowClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);//�����������ʹ��Ĭ�Ϲ��
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//������ˢ�������ɫֵ�����Ǳ�׼ϵͳ��ɫ֮һ���ұ����1
	windowClass.lpszMenuName = nullptr;//ָ��˵����Ƶ��ַ��������ﲻʹ�ò˵�������Ϊnullptr
	windowClass.lpszClassName = windowClassName;//ָ�򴰿��������ַ���
	windowClass.hIconSm = ::LoadIcon(hInst, nullptr);//�봰���������Сͼ����

	static ATOM atom = ::RegisterClassExW(&windowClass);
	assert(atom > 0);//atomֵ����0��ע��ɹ�������ע��ʧ���׳��쳣
}

//��������
HWND CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height) {
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);//��ȡ��Ļ���
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);//��ȡ��Ļ�߶�

	RECT windowRect = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);//���ݴ�����ʽ�������ھ���

	int windowWidth = windowRect.right - windowRect.left;//������Ĵ��ڿ��
	int windowHeight = windowRect.bottom - windowRect.top;//������Ĵ��ڸ߶�

	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);//�������Ͻ�x����
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);//�������Ͻ�y����

	HWND hWnd = ::CreateWindowExW(
		NULL,//��չ������ʽ
		windowClassName,//��������
		windowTitle,//���ڱ���
		WS_OVERLAPPEDWINDOW,//������ʽ
		windowX,//���ڳ�ʼˮƽλ��
		windowY,//���ڳ�ʼ��ֱλ��
		windowWidth,//���ڿ��
		windowHeight,//���ڸ߶�
		nullptr,//�����ھ��
		nullptr,//�˵����
		hInst,//Ӧ��ʵ�����
		nullptr//ָ�򴰿ڴ������ݵ�ָ��
	);

	assert(hWnd && "Failed to create window");

	return hWnd;
}

//��ѯDirectX12������
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

			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_NONE) == 0 && //�����������������
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) && //�����������D3D��֧�ּ��𣬷��ؽӿڵĲ�����ֵΪnullptr�������Ͳ��ᴴ���豸��ֻ�Ǽ��֧�ּ���
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory) { //����ѡ���ռ�Դ�����������
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}
	return dxgiAdapter4;
}

//����DirectX12�豸
ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter) {
	ComPtr<ID3D12Device2> d3d12Device2;
	ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(d3d12Device2.As(&pInfoQueue))) {
		//��������Ϣ���ص����¼���ʱ�������ж�
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);//�ڴ���ʱ����
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
	}
	//��Ȼ�ڷ���Ӧ�ó���ǰӦ������о��棬���ǽ�����п��ܷ����ľ��沢����ʵ�����Կ���ʹ�ù�����������һЩ������Ϣ
	//�������к���
	//D3D12_MESSAGE_CATEGORY Categories[] = {};
	//����Ϣ�����Լ�����������Ϣ
	D3D12_MESSAGE_SEVERITY Severities[] = {
		D3D12_MESSAGE_SEVERITY_INFO
	};
	//ͨ��ID���Ե�����Ϣ
	D3D12_MESSAGE_ID DenyIds[] = {
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,//��ʹ�ò�������Դ�����ڼ�ָ�����Ż�͸����ɫ��͸����ɫ�����ȾĿ��ʱ������ִ˾���
		D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE, //��ͼ�ε���ʱʹ�ò���֡ʱ����ִ˾���
		D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE //��ͼ�ε���ʱʹ�ò���֡ʱ����ִ˾���
	};

	D3D12_INFO_QUEUE_FILTER NewFilter = {};
	//NewFilter.DenyList.NumCategories = _countof(Categories);
	//NewFilter.DenyList.pCategoryList = Categories;
	NewFilter.DenyList.NumSeverities = _countof(Severities);
	NewFilter.DenyList.pSeverityList = Severities;
	NewFilter.DenyList.NumIDs = _countof(DenyIds);
	NewFilter.DenyList.pIDList = DenyIds;

	ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));//�����������͵���Ϣ������
#endif
	return d3d12Device2;
}

//�����������
ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12CommandQueue> d3d12CommandQueue;
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;//��GPUʱΪ0

	ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
	return d3d12CommandQueue;
}

//˺��֧�ּ��
bool CheckTearingSupport() {
	BOOL allowTearing = FALSE;
	//ͼ�ε��Թ��߲�֧��DXGI1.5�������ȴ���DXGI1.4�ӿڣ�Ȼ��ʹ��As��ȡDXGI1.5�ӿ�
	ComPtr<IDXGIFactory4> factory4;//IDXGIFactory4������CheckFeatureSupport����
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

//����������
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
	swapChainDesc.Stereo = FALSE;//�Ƿ������������������������Ҫָ��flip-mode������
	swapChainDesc.SampleDesc = { 1,0 };//��ʹ��flip-mode������ʱ������ָ��Ϊ{1,0}
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//������ȾĿ�����
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;//ָʾDXGI����̨�����������ݽ�����������Ӧ��ʾĿ��Ĵ�С
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//����Ϊ��תģʽ�����Ҳ�����֮ǰ���ֵ�֡
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;//��ʶ��������̨��������͸������Ϊ��������δָ��
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;//����Ϊ֧����Ļ˺��

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(//���ڴ��������ϵͳ���ھ�������Ľ�����
		commandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,//��ʹ��ȫ��ģʽ������Ϊnullptr
		nullptr,//�����Ƶ��ض������������Ϊnullptr
		&swapChain1//ֻ����IDXGISwapChain1�ӿ�
	));
	//��ֹALT+ENTERȫ���л�
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));//�ֶ������Ի���ޱ߿򴰿�

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	return dxgiSwapChain4;
}

//������������
ComPtr<ID3D12DescriptorHeap> CreateDescritorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors) {
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;

	ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	return descriptorHeap;
}

//����RTV
void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap) {
	auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < g_NumFrames; ++i) {
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));//��ȡָ�򽻻�����̨��������ָ��
		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);//nullptr������Ĭ��������

		g_BackBuffers[i] = backBuffer;

		rtvHandle.Offset(rtvDescriptorSize);//ƫ�Ƶ���һ����`����
	}
}

//�������������
ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
	return commandAllocator;
}

//���������б�
ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type) {
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
	ThrowIfFailed(commandList->Close());
	return commandList;
}

//����դ��
ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device) {
	ComPtr<ID3D12Fence> fence;
	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	return fence;
}

//��������ϵͳ�¼�
HANDLE CreateEventHandle() {
	HANDLE fenceEvent;
	fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);//����һ���Զ����õĳ�ʼ״̬Ϊ���źŵ��¼�
	assert(fenceEvent && "Failed to create fence event.");
	return fenceEvent;
}

//Signal�źź���
uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue) {
	//�ú�����GPU��դ�������źţ�ֻ����GPU���������ִ�й����дﵽ�õ�ʱ�Żᷢ���ź�
	uint64_t fenceValueForSignal = ++fenceValue;
	ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));
	return fenceValueForSignal;
}

//Wait�ȴ�����
void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::microseconds duration = std::chrono::milliseconds::max()) {
	if (fence->GetCompletedValue() < fenceValue) {
		ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));//ָ���ﵽfenceValueʱ������fenceEvent�¼�
		::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));//�����̵߳ȴ��¼��������߳�ʱ
	}
}

//Flushˢ�º���
void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent) {
	//�ú�������ȷ��֮ǰ��GPU��ִ�е��κ������ִ����ϣ��ú������غ���԰�ȫ���ͷ���Դ
	uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
	WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}

//Update���º���
void Update() {
	static uint64_t frameCounter = 0;
	static double elapsedSeconds = 0.0;
	static std::chrono::high_resolution_clock clock;//ʱ�ӱ���Ϊhigh_resolution_clock������ʱ���Ϊstd::chrono::time_point
	static auto t0 = clock.now();

	++frameCounter;
	auto t1 = clock.now();//Ĭ�ϵ�λΪnanosecond
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

//Render��Ⱦ����
void Render() {
	auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];
	//��������������������б�
	commandAllocator->Reset();
	g_CommandList->Reset(commandAllocator.Get(), nullptr);
	//Clear���
	{
		//�������ȾĿ��ǰ��Ҫ�Ƚ���ת��ΪRENDER_TARGET״̬
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		g_CommandList->ResourceBarrier(1, &barrier);//������Դ����

		FLOAT clearColor[] = { 0.4f,0.6f,0.9f,1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), g_CurrentBackBufferIndex, g_RTVDescriptorSize);//��ȡ��ǰ֡��RTV���������
		g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);//nullptr��ʾ���������Դ��ͼ
	}
	//Present����
	{
		//�ڳ���ǰ��Ҫ�Ƚ���ת��ΪPRESENT״̬
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
		//Signal���ص�դ��ֵ����������CPU�̣߳�ֱ����Դʹ�����
		g_FrameFenceValues[g_CurrentBackBufferIndex] = Signal(g_CommandQueue, g_Fence, g_FenceValue);//���µ�ǰ֡��դ��ֵ
		g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();//��Present֮���̨���������������ܸı�
		WaitForFenceValue(g_Fence, g_FrameFenceValues[g_CurrentBackBufferIndex], g_FenceEvent);
	}
}

//Resize���ô�С����
void Resize(uint32_t width, uint32_t height) {
	if (g_ClientWidth != width || g_ClientHeight != height) {
		g_ClientHeight = std::max(1u, width);
		g_ClientWidth = std::max(1u, height);
		//�ڵ�����������Сʱ����Ҫ�ͷŶԽ�������̨�����������ã�������GPU�Ͽ�����һ�������е������б������˽������ĺ�̨��������������Ҫ��ˢ��GPU
		Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
		for (int i = 0; i < g_NumFrames; ++i) {
			g_BackBuffers[i].Reset();//���ú�̨������
			g_FrameFenceValues[i] = g_FrameFenceValues[g_CurrentBackBufferIndex];//��դ��ֵ����Ϊ��ǰ֡��դ��ֵ
		}
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		ThrowIfFailed(g_SwapChain->GetDesc(&swapChainDesc));
		ThrowIfFailed(g_SwapChain->ResizeBuffers(g_NumFrames, g_ClientWidth, g_ClientHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
		//�ڵ�����������С�󣬺�̨���������������ܸı䣬�������½��л�ȡ���������µ�RTV
		g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
		UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
	}
}

//ȫ���л�����
void SetFullScreen(bool fullscreen) {
	if (g_Fullsceen != fullscreen) {
		g_Fullsceen = fullscreen;
		if (g_Fullsceen) {
			::GetWindowRect(g_HWnd, &g_WindowRect);
			//��������ʽ����Ϊ�ޱ߿򴰿�
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			::SetWindowLongW(g_HWnd, GWL_STYLE, windowStyle);//GWL_STYLE��ʾ���ô�����ʽ

			HMONITOR hMoniter = ::MonitorFromWindow(g_HWnd, MONITOR_DEFAULTTONEAREST);//��������Ӧ�ó��򴰿��������ʾ���ľ��
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMoniter, &monitorInfo);//��ȡ��ʾ������Ϣ
			::SetWindowPos(g_HWnd, HWND_TOP,//HWND_TOP��ʾ����������Z��Ķ���
				monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				//SWP_FRAMECHANGED��־��ʹ��SetWindowLong�������Ĵ��ڵĿ����ʽʱ������ʹ�ô˱�־��Ӧ���µĿ����ʽ��
				//�����򴰿ڷ���һ��WM_NCCALCSIZE��Ϣ����ʹ���ڵĴ�Сû�иı�Ҳ�ᷢ�ʹ���Ϣ��
				//���û��ָ���˱�־����ֻ���ڴ��ڴ�С�ı�ʱ�Żᷢ��WM_NCCALCSIZE��Ϣ
				//SWP_NOACTIVATE��־�����ڵ���SetWindowPos����ʱ������ڡ�
				//���û�����ô˱�־�����ڽ�������ƶ�������������Ķ���(����ȡ����hWndInsertAfter����������)
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(g_HWnd, SW_MAXIMIZE);//��󻯴���
		}
		else {
			::SetWindowLong(g_HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//���Ļ�ԭ�����ڵ���ʽ
			::SetWindowPos(g_HWnd, HWND_NOTOPMOST,
				g_WindowRect.left,
				g_WindowRect.top,
				g_WindowRect.right - g_WindowRect.left,
				g_WindowRect.bottom - g_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(g_HWnd, SW_NORMAL);//�����ʾ����
		}
	}
}

//���ڹ��̺���
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
			//���δ����WM_SYSCHAR��Ϣ����Ĭ�ϴ��ڹ��̽��������Ϣ�����ڴ����м��̽���ʱ����ϵͳ֪ͨ��
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
			//�������Ӧ����Ϣ�����ܵ���Ӧ��ʵ���޷��˳�
			::PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProcW(hWnd, message, wParam, lParam);//ʹ��Ĭ�ϴ��ڹ��̴�����Ϣ
		}
	}
	else {
		return ::DefWindowProcW(hWnd, message, wParam, lParam);//ʹ��Ĭ�ϴ��ڹ��̴�����Ϣ
	}
	return 0;
}

//����ڵ�
int CALLBACK wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
	//Windows 10 Creators ��������� Per Monitor V2 DPI ��֪������
	//ʹ�ô˸�֪�����������ڵĿͻ�������ʵ�� 100% ���ţ�ͬʱ��Ȼ������ DPI ���з�ʽ���ַǿͻ��˴�������
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);//�����̵߳�DPI��֪������
	const wchar_t* windowClassName = L"DX12WindowClass";//��������
	ParseCommandLineArguments();//���������в���
	EnableDebugLayer();//���õ��Բ�
	g_TearingSupported = CheckTearingSupport();//���˺��֧��
	RegisterWindowClass(hinstance, windowClassName);//ע�ᴰ����
	g_HWnd = CreateWindow(windowClassName, hinstance, L"Learn Dx12", g_ClientWidth, g_ClientHeight);//��������
	::GetWindowRect(g_HWnd, &g_WindowRect);//��ȡ���ھ���

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











