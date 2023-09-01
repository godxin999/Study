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
			//��������ʽ����Ϊ�ޱ߿򴰿�
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			::SetWindowLongW(m_hWnd, GWL_STYLE, windowStyle);//GWL_STYLE��ʾ���ô�����ʽ

			HMONITOR hMoniter = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);//��������Ӧ�ó��򴰿��������ʾ���ľ��
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMoniter, &monitorInfo);//��ȡ��ʾ������Ϣ
			::SetWindowPos(m_hWnd, HWND_TOP,//HWND_TOP��ʾ����������Z��Ķ���
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
			::ShowWindow(m_hWnd, SW_MAXIMIZE);//��󻯴���
		}
		else {
			::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//���Ļ�ԭ�����ڵ���ʽ
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST,
				m_WindowRect.left,
				m_WindowRect.top,
				m_WindowRect.right - m_WindowRect.left,
				m_WindowRect.bottom - m_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE
			);
			::ShowWindow(m_hWnd, SW_NORMAL);//�����ʾ����
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
	swapChainDesc.Stereo = FALSE;//�Ƿ������������������������Ҫָ��flip-mode������
	swapChainDesc.SampleDesc = { 1,0 };//��ʹ��flip-mode������ʱ������ָ��Ϊ{1,0}
	swapChainDesc.BufferCount = BufferCount;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//������ȾĿ�����
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;//ָʾDXGI����̨�����������ݽ�����������Ӧ��ʾĿ��Ĵ�С
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//����Ϊ��תģʽ�����Ҳ�����֮ǰ���ֵ�֡
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;//��ʶ��������̨��������͸������Ϊ��������δָ��
	swapChainDesc.Flags = m_IsTearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;//����Ϊ֧����Ļ˺��

	ID3D12CommandQueue* pCommandQueue = app.GetCommandQueue()->GetD3D12CommandQueue().Get();

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(//���ڴ��������ϵͳ���ھ�������Ľ�����
		pCommandQueue,
		m_hWnd,
		&swapChainDesc,
		nullptr,//��ʹ��ȫ��ģʽ������Ϊnullptr
		nullptr,//�����Ƶ��ض������������Ϊnullptr
		&swapChain1//ֻ����IDXGISwapChain1�ӿ�
	));
	//��ֹALT+ENTERȫ���л�
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER));//�ֶ������Ի���ޱ߿򴰿�

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	m_CurrentBackBufferIndex = dxgiSwapChain4->GetCurrentBackBufferIndex();

	return dxgiSwapChain4;
}


void Window::UpdateRenderTargetViews() {
	auto device = Application::Get().GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_d3d12RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < BufferCount; ++i) {
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));//��ȡָ�򽻻�����̨��������ָ��
		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);//nullptr������Ĭ��������

		m_d3d12BackBuffers[i] = backBuffer;

		rtvHandle.Offset(m_RTVDescriptorSize);//ƫ�Ƶ���һ����`����
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

