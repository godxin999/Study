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
	static Application& Get();//��ȡӦ�ó�����

	static void Create(HINSTANCE hInst);//ͨ��ʾ���������Ӧ�õ���
	static void Destory();//����Ӧ�ó���ʵ������Ӧ��ʵ�����������д���
	
	bool IsTearingSupported()const;//����Ƿ�֧��˺��

	std::shared_ptr<Window> CreateRenderWindow(const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync = true);//��������
	void DestroyWindow(const std::wstring& windowName);//ͨ�������������ٴ���
	void DestroyWindow(std::shared_ptr<Window> window);//ͨ������ָ�����ٴ���

	std::shared_ptr<Window> GetWindowByName(const std::wstring& windowName);//���ݴ������ƻ�ȡ����ָ��
	
	int Run(std::shared_ptr<Game> pGame);
	void Quit(int exitCode = 0);
	
	Microsoft::WRL::ComPtr<ID3D12Device2> GetDevice()const;
	std::shared_ptr<CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type=D3D12_COMMAND_LIST_TYPE_DIRECT)const;

	void Flush();//ˢ�������������
	
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