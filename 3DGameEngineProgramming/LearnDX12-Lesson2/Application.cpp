#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Game.h"
#include "CommandQueue.h"

constexpr wchar_t WINDOW_CLASS_NAME[] = L"DX12RenderWindowClass";

using WindowPtr = std::shared_ptr<Window>;
using WindowMap = std::map<HWND, WindowPtr>;
using WindowNameMap = std::map<std::wstring, WindowPtr>;

static Application* gs_pSingelton = nullptr;
static WindowMap gs_Windows;
static WindowNameMap gs_WindowByName;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

struct MakeWindow :public Window {
	MakeWindow(HWND hWnd, const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync) :
		Window(hWnd, windowName, clientWidth, clientHeight, vSync) {}
};

Application::Application(HINSTANCE hInst) :m_hInstance(hInst), m_TearingSupported(false) {
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);//�����̵߳�DPI��֪������

#if defined(_DEBUG)
	//�ڴ���ID3D12Device֮ǰ���õ��Բ㣬���򽫻ᵼ������ʱɾ���豸
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif

	WNDCLASSEXW wndClass = {};

	wndClass.cbSize = sizeof(WNDCLASSEXW);//�ṹ���С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//������ʽ������Ϊ�����ݷ����б仯ʱ�ػ�
	wndClass.lpfnWndProc = &WndProc;//���ڻص�����
	wndClass.cbClsExtra = 0;//������ĸ����ڴ棬һ��Ϊ0
	wndClass.cbWndExtra = 0;//���ڵĸ����ڴ棬һ��Ϊ0
	wndClass.hInstance = hInst;//Ӧ��ʵ�����
	wndClass.hIcon = ::LoadIcon(hInst, nullptr);//ͼ����������ʹ��LoadIcon��������Դ�ļ�����ͼ�ֵ꣬Ϊnullptr��ʹ��Ĭ��ͼ��
	wndClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);//�����������ʹ��Ĭ�Ϲ��
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//������ˢ�������ɫֵ�����Ǳ�׼ϵͳ��ɫ֮һ���ұ����1
	wndClass.lpszMenuName = nullptr;//ָ��˵����Ƶ��ַ��������ﲻʹ�ò˵�������Ϊnullptr
	wndClass.lpszClassName = WINDOW_CLASS_NAME;//ָ�򴰿��������ַ���
	wndClass.hIconSm = ::LoadIcon(hInst, nullptr);//�봰���������Сͼ����

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

void Application::Destroy() {
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

Microsoft::WRL::ComPtr<ID3D12Device2> Application::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter) {
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

bool Application::CheckTearingSupport() {
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
		window->Destroy();
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

static void RemoveWindow(HWND hWnd) {
	auto windowIter = gs_Windows.find(hWnd);
	if (windowIter != gs_Windows.end()) {
		auto pWindow = windowIter->second;
		gs_WindowByName.erase(pWindow->GetWindowName());
		gs_Windows.erase(windowIter);
	}
}

MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID){
	//������Ϣֵ���ض�Ӧ����갴��
	MouseButtonEventArgs::MouseButton mouseButton = MouseButtonEventArgs::None;
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Left;
	}
	break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Right;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Middle;
	}
	break;
	}

	return mouseButton;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	WindowPtr pWindow;
	{
		WindowMap::iterator iter = gs_Windows.find(hwnd);
		if (iter != gs_Windows.end()){
			pWindow = iter->second;
		}
	}

	if (pWindow){
		switch (message)
		{
		case WM_PAINT:
		{
			UpdateEventArgs updateEventArgs(0.0f, 0.0f);
			pWindow->OnUpdate(updateEventArgs);
			RenderEventArgs renderEventArgs(0.0f, 0.0f);
			pWindow->OnRender(renderEventArgs);
		}
		break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			MSG charMsg;
			unsigned int c = 0;
			//���ڿɴ�ӡ�ַ�����һ����ϢΪWM_CHAR��������Ϣ����Ҫ���͸�KeyPressedEvent���ַ�����
			//PeekMessage�����Ϣ���У��������������Ϣ���򽫸���Ϣ���Ƶ�ָ���Ľṹ�У���������Ӷ�����ɾ�����ú����������̷��ز��������߳�
			//PM_NOREMOVEָʾ�ڼ����Ϣ���к󣬲�����Ϣ�Ӷ�����ɾ��
			if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR){
				//�Ӷ����л�ȡ��Ϣ������ɾ��
				GetMessage(&charMsg, hwnd, 0, 0);
				c = static_cast<unsigned int>(charMsg.wParam);//��ȡ�ַ�����
			}
			//GetAsyncKeyState����ͨ��������Ӳ���ж�����ȡ�����״̬(���»��ͷ�)
			//��ĸ�����ֵ����������Ϊ��Ӧ��ASCII�룬�������ж�Ӧ����������룬��ʹ��VK��ͷ�ĺ��ʾ����VK_SHIFT��ʾShift��
			//GetAsyncKeyState��������ֵ�����λ��ʾ���Ƿ񱻰��£���������������λΪ1������Ϊ0����Ϊ����ֵ��short���ͣ�������Ҫ&0x8000
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode::Key key = (KeyCode::Key)wParam;//��ȡ���������
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;//��16-23λ��ʾɨ���룬����ͨ��λ�����ȡɨ����

			KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Pressed, shift, ctrl, alt);
			pWindow->OnKeyPressed(keyEventArgs);
		}
		break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode::Key key = (KeyCode::Key)wParam;
			unsigned int c = 0;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
			//��ȡ��ǰ����״̬��GetKeyBoardState�������ú󣬼���״̬�ᱻ��䵽ָ����������
			//�������������£��������ж�ӦԪ�ص����λ����1������Ϊ0
			unsigned char keyboardState[256];
			GetKeyboardState(keyboardState);
			//ToUnicodeEx������ָ��������������ɨ����ת��ΪUnicode�ַ�������ֵΪ��Ч�ַ�λ������Ч�ַ�λ�ڻ�������λ
			wchar_t translatedCharacters[4];//�洢ת������ַ�
			if (int result = ToUnicodeEx(static_cast<UINT>(wParam), scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0){
				c = translatedCharacters[0];//��ȡת������ַ�
			}

			KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Released, shift, control, alt);
			pWindow->OnKeyReleased(keyEventArgs);
		}
		break;
		case WM_SYSCHAR:
			//���δ����WM_SYSCHAR��Ϣ����Ĭ�ϴ��ڹ��̽��������Ϣ�����ڴ����м��̽���ʱ����ϵͳ֪ͨ�����簴��Alt+Enterʱ
			break;
		case WM_MOUSEMOVE:
		{
			//�ڴ���WM_MOUSEMOVE��Ϣʱ��wParam����������갴ť�ͼ������μ�״̬����Ϣ
			//ʹ��wParam�Ͷ�Ӧ���ĺ�(MK_)����λ���㣬��������Ϊ0�����ʾ�ü�������
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;
			//ʹ��LOWORD��HIWORD���ȡ���λ��
			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseMoved(mouseMotionEventArgs);
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseButtonPressed(mouseButtonEventArgs);
		}
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseButtonReleased(mouseButtonEventArgs);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			//HIWORD��wParam�л�ȡ�����ֵĹ�������������ʾ���Ϲ�����������ʾ���¹���
			//WHEEL_DELTA��Ϊ�����ֹ���һ�еĹ����������Ը�ֵ�õ�����������
			//LOWORD��wParam�л�ȡ�����ֹ���ʱ�ļ������μ�״̬
			float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
			short keyStates = (short)LOWORD(wParam);

			bool lButton = (keyStates & MK_LBUTTON) != 0;
			bool rButton = (keyStates & MK_RBUTTON) != 0;
			bool mButton = (keyStates & MK_MBUTTON) != 0;
			bool shift = (keyStates & MK_SHIFT) != 0;
			bool control = (keyStates & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			//���ͻ�������ת��Ϊ��Ļ����
			POINT clientToScreenPoint;
			clientToScreenPoint.x = x;
			clientToScreenPoint.y = y;
			ScreenToClient(hwnd, &clientToScreenPoint);

			MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
			pWindow->OnMouseWheel(mouseWheelEventArgs);
		}
		break;
		case WM_SIZE:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			ResizeEventArgs resizeEventArgs(width, height);
			pWindow->OnResize(resizeEventArgs);
		}
		break;
		case WM_DESTROY:
		{
			//��ȫ�ִ����б����Ƴ�����
			RemoveWindow(hwnd);

			if (gs_Windows.empty()){
				//���û������Ĵ��ڣ����˳�����
				PostQuitMessage(0);
			}
		}
		break;
		default:
			//����Ĭ�ϴ��ڹ��̴����������Ϣ
			return DefWindowProcW(hwnd, message, wParam, lParam);
		}
	}
	else{
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}

	return 0;
}