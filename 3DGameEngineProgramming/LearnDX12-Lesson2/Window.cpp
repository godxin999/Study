#include "pch.h"
#include "Window.h"
#include "Application.h"
#include "CommandQueue.h"
#include "Game.h"


Window::Window(HWND hWnd, const std::wstring& windowName, uint32_t clientWidth, uint32_t clientHeight, bool vSync) :
	m_hWnd(hWnd),
	m_WindowName(windowName),
	m_ClientWidth(clientWidth),
	m_ClientHeight(clientHeight),
	m_VSync(vSync) {
	Application& app = Application::Get();

	m_IsTearingSupported = app.IsTearingSupported();

	m_dxgiSwapChain = CreateSwapChain();
	m_d3d12RTVDescriptorHeap = app.CreateDescriptorHeap(BufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_RTVDescriptorSize = app.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews();
}

Window::~Window() {
	assert(!m_hWnd && "Use Application::DestroyWindow before destruction");
}

void Window::RegisterCallbacks(std::shared_ptr<Game> pGame) {
	m_pGame = pGame;
	return;
}

HWND Window::GetWindowHandle()const {
	return m_hWnd;
}

const std::wstring& Window::GetWindowName()const {
	return m_WindowName;
}

void Window::Show() {
	::ShowWindow(m_hWnd, SW_SHOW);
}

void Window::Hide() {
	::ShowWindow(m_hWnd, SW_HIDE);
}

void Window::Destory() {
	if (auto pGame = m_pGame.lock()) {
		pGame->OnWindowDestroy();
	}
	else {
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

int Window::GetClientHeight()const {
	return m_ClientHeight;
}

int Window::GetClientWidth()const {
	return m_ClientWidth;
}

bool Window::isVSync()const {
	return m_VSync;
}

void Window::SetVSync(bool vSync) {
	m_VSync = vSync;
}

void Window::ToggleVSync() {
	SetVSync(!m_VSync);
}

bool Window::isFullScreen()const {
	return m_FullScreen;
}

void Window::SetFullScreen(bool fullScreen) {
	if (m_FullScreen != fullScreen) {
		m_FullScreen = fullScreen;
		if (m_FullScreen) {
			::GetWindowRect(m_hWnd, &m_WindowRect);
			//将窗口样式设置为无边框窗口
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			::SetWindowLongW(m_hWnd, GWL_STYLE, windowStyle);//GWL_STYLE表示设置窗口样式

			HMONITOR hMoniter = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);//检索距离应用程序窗口最近的显示器的句柄
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMoniter, &monitorInfo);//获取显示器的信息
			::SetWindowPos(m_hWnd, HWND_TOP,//HWND_TOP表示将窗口置于Z序的顶部
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
			::ShowWindow(m_hWnd, SW_MAXIMIZE);//最大化窗口
		}
		else {
			::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//更改回原来窗口的样式
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST,
				m_WindowRect.left,
				m_WindowRect.top,
				m_WindowRect.right - m_WindowRect.left,
				m_WindowRect.bottom - m_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(m_hWnd, SW_NORMAL);//激活并显示窗口
		}
	}
}

void Window::ToggleFullScreen() {
	SetFullScreen(!m_FullScreen);
}


Microsoft::WRL::ComPtr<IDXGISwapChain4> Window::CreateSwapChain() {
	Application& app = Application::Get();

	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory4> dxgiFactory4;

	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = m_ClientWidth;
	swapChainDesc.Height = m_ClientHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;//是否是立体声，如果是立体声还要指定flip-mode交换链
	swapChainDesc.SampleDesc = { 1,0 };//当使用flip-mode交换链时，必须指定为{1,0}
	swapChainDesc.BufferCount = BufferCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//用于渲染目标输出
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;//指示DXGI将后台缓冲区的内容进行缩放以适应演示目标的大小
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//设置为翻转模式，并且不保留之前呈现的帧
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;//标识交换链后台缓冲区的透明度行为，这里是未指定
	swapChainDesc.Flags = m_IsTearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;//设置为支持屏幕撕裂

	ID3D12CommandQueue* pCommandQueue = app.GetCommandQueue()->GetD3D12CommandQueue().Get();

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(//用于创建与操作系统窗口句柄关联的交换链
		pCommandQueue,
		m_hWnd,
		&swapChainDesc,
		nullptr,//不使用全屏模式，设置为nullptr
		nullptr,//不限制到特定的输出，设置为nullptr
		&swapChain1//只接受IDXGISwapChain1接口
	));
	//禁止ALT+ENTER全屏切换
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER));//手动处理以获得无边框窗口

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	m_CurrentBackBufferIndex = dxgiSwapChain4->GetCurrentBackBufferIndex();

	return dxgiSwapChain4;
}


void Window::UpdateRenderTargetViews() {
	auto device = Application::Get().GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_d3d12RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < BufferCount; ++i) {
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));//获取指向交换链后台缓冲区的指针
		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);//nullptr代表创建默认描述符

		m_d3d12BackBuffers[i] = backBuffer;

		rtvHandle.Offset(m_RTVDescriptorSize);//偏移到下一个描`述符
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE Window::GetCurrentRenderTargetView()const {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_d3d12RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrentBackBufferIndex, m_RTVDescriptorSize);
}

Microsoft::WRL::ComPtr<ID3D12Resource> Window::GetCurrentBackBuffer()const {
	return m_d3d12BackBuffers[m_CurrentBackBufferIndex];
}

UINT Window::GetCurrentBackBufferIndex()const {
	return m_CurrentBackBufferIndex;
}

UINT Window::Present(){
	UINT syncInterval = m_VSync ? 1 : 0;
	UINT presentFlags = m_IsTearingSupported && !m_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
	ThrowIfFailed(m_dxgiSwapChain->Present(syncInterval, presentFlags));
	m_CurrentBackBufferIndex= m_dxgiSwapChain->GetCurrentBackBufferIndex();
	return m_CurrentBackBufferIndex;
}

