#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <string>

class Window;
class CommandQueue;
class Game;
class Application {
public:
	static Application& Get();//获取应用程序单例

	static void Create(HINSTANCE hInst);//通过示例句柄创建应用单例
	static void Destory();//销毁应用程序实例及该应用实例创建的所有窗口
	
	bool IsTearingSupported()const;//检查是否支持撕裂

	std::shared_ptr<Window> CreateRenderWindow(const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync = true);//创建窗口
	void DestroyWindow(const std::wstring& windowName);//通过窗口名称销毁窗口
	void DestroyWindow(std::shared_ptr<Window> window);//通过窗口指针销毁窗口

	std::shared_ptr<Window> GetWindowByName(const std::wstring& windowName);//根据窗口名称获取窗口指针
	
	int Run(std::shared_ptr<Game> pGame);
	void Quit(int exitCode = 0);
	
	Microsoft::WRL::ComPtr<ID3D12Device2> GetDevice()const;
	std::shared_ptr<CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type=D3D12_COMMAND_LIST_TYPE_DIRECT)const;

	void Flush();//刷新所有命令队列
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type);
	
	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type)const;
protected:
	Application(HINSTANCE hInst);
	virtual ~Application();

	Microsoft::WRL::ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	Microsoft::WRL::ComPtr<ID3D12Device2> CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);

	bool CheckTearingSupport();
private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	HINSTANCE m_hInstance;

	Microsoft::WRL::ComPtr<IDXGIAdapter4> m_dxgiAdapter;
	Microsoft::WRL::ComPtr<ID3D12Device2> m_d3d12Device;

	std::shared_ptr<CommandQueue> m_DirectCommandQueue;
	std::shared_ptr<CommandQueue> m_ComputeCommandQueue;
	std::shared_ptr<CommandQueue> m_CopyCommandQueue;

	bool m_TearingSupported;
};