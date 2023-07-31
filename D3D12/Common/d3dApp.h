//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3DApp
{
protected:

	D3DApp(HINSTANCE hInstance);
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	virtual ~D3DApp();

public:

	static D3DApp* GetApp();

	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);

	int Run();
	//六个框架虚函数，每次重写
	virtual bool Initialize();//初始化代码
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//窗口过程函数

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();//创建应用程序所需的RTV和DSV描述符堆
	virtual void OnResize();//窗口调整函数
	virtual void Update(const GameTimer& gt) = 0;//更新函数
	virtual void Draw(const GameTimer& gt) = 0;//绘制函数

	//处理树苗消息的虚函数
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:

	bool InitMainWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void CalculateFrameStats();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

	static D3DApp* mApp;

	HINSTANCE mhAppInst = nullptr;//应用程序实例句柄
	HWND      mhMainWnd = nullptr;//主窗口句柄
	bool      mAppPaused = false;//应用程序是否暂停
	bool      mMinimized = false;//应用程序是否最小化
	bool      mMaximized = false;//应用程序是否最大化
	bool      mResizing = false;//大小调整栏是否收到拖拽
	bool      mFullscreenState = false;//是否开启全屏模式

	//设置为真则开启4X MSAA，默认为false
	bool      m4xMsaaState = false;//是否开启4X MSAA
	UINT      m4xMsaaQuality = 0;//质量级别

	//用于记录帧之间的时间间隔和游戏总时间
	GameTimer mTimer;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;//工厂
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;//交换链
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;//设备

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;//围栏
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//命令队列
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;//命令分配器
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//执行命令列表

	static const int SwapChainBufferCount = 2;//交换链缓冲区数量
	int mCurrBackBuffer = 0;//当前后台缓冲区的索引
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
	//描述符堆
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;//存储渲染目标视图RTV
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;//存储深度/模板视图DSV

	D3D12_VIEWPORT mScreenViewport;//视口
	D3D12_RECT mScissorRect;//裁剪矩形
	//描述符大小(占字节数)
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	//在D3DApp派生类中自定义这些初始值
	std::wstring mMainWndCaption = L"MainWindow";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;
};

