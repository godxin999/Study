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
	//根据消息值返回对应的鼠标按键
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
			//对于可打印字符，下一条消息为WM_CHAR，这条消息包含要发送给KeyPressedEvent的字符编码
			//PeekMessage检查消息队列，如果队列中有消息，则将该消息复制到指定的结构中，但不将其从队列中删除，该函数检查后立刻返回不会阻塞线程
			//PM_NOREMOVE指示在检查消息队列后，不将消息从队列中删除
			if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR){
				//从队列中获取消息并将其删除
				GetMessage(&charMsg, hwnd, 0, 0);
				c = static_cast<unsigned int>(charMsg.wParam);//获取字符编码
			}
			//GetAsyncKeyState函数通过侦测键盘硬件中断来获取物理键状态(按下或释放)
			//字母或数字的虚拟键编码为对应的ASCII码，其他键有对应的虚拟键编码，并使用VK开头的宏表示，如VK_SHIFT表示Shift键
			//GetAsyncKeyState函数返回值的最高位表示键是否被按下，如果被按下则最高位为1，否则为0，因为返回值是short类型，所以需要&0x8000
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode::Key key = (KeyCode::Key)wParam;//获取虚拟键编码
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;//第16-23位表示扫描码，这里通过位运算获取扫描码

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
			//获取当前键盘状态，GetKeyBoardState函数调用后，键盘状态会被填充到指定的数组中
			//如果虚拟键被按下，则数组中对应元素的最高位被置1，否则为0
			unsigned char keyboardState[256];
			GetKeyboardState(keyboardState);
			//ToUnicodeEx函数将指定的虚拟键编码和扫描码转换为Unicode字符，返回值为有效字符位数，有效字符位于缓冲区低位
			wchar_t translatedCharacters[4];//存储转换后的字符
			if (int result = ToUnicodeEx(static_cast<UINT>(wParam), scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0){
				c = translatedCharacters[0];//获取转换后的字符
			}

			KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Released, shift, control, alt);
			pWindow->OnKeyReleased(keyEventArgs);
		}
		break;
		case WM_SYSCHAR:
			//如果未处理WM_SYSCHAR消息，则默认窗口过程将处理该消息，并在窗口有键盘焦点时播放系统通知音，如按下Alt+Enter时
			break;
		case WM_MOUSEMOVE:
		{
			//在处理WM_MOUSEMOVE消息时，wParam参数包含鼠标按钮和键盘修饰键状态的信息
			//使用wParam和对应键的宏(MK_)进行位运算，如果结果不为0，则表示该键被按下
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;
			//使用LOWORD和HIWORD宏获取鼠标位置
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
			//HIWORD从wParam中获取鼠标滚轮的滚动量，正数表示向上滚动，负数表示向下滚动
			//WHEEL_DELTA宏为鼠标滚轮滚动一行的滚动量，除以该值得到滚动的行数
			//LOWORD从wParam中获取鼠标滚轮滚动时的键盘修饰键状态
			float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
			short keyStates = (short)LOWORD(wParam);

			bool lButton = (keyStates & MK_LBUTTON) != 0;
			bool rButton = (keyStates & MK_RBUTTON) != 0;
			bool mButton = (keyStates & MK_MBUTTON) != 0;
			bool shift = (keyStates & MK_SHIFT) != 0;
			bool control = (keyStates & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			//将客户端坐标转换为屏幕坐标
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
			//从全局窗口列表中移除窗口
			RemoveWindow(hwnd);

			if (gs_Windows.empty()){
				//如果没有其余的窗口，就退出程序
				PostQuitMessage(0);
			}
		}
		break;
		default:
			//调用默认窗口过程处理其余的消息
			return DefWindowProcW(hwnd, message, wParam, lParam);
		}
	}
	else{
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}

	return 0;
}