#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <string>

#include "Events.h"
#include "HighResolutionClock.h"

class Game;
class Window {
public:
	static const UINT BufferCount = 3;

	HWND GetWindowHandle()const;
	const std::wstring& GetWindowName()const;
	int GetClientWidth()const;
	int GetClientHeight()const;
	UINT GetCurrentBackBufferIndex()const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRenderTargetView()const;
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentBackBuffer()const;

	void Destroy();

	bool isVSync()const;
	void SetVSync(bool vSync);
	void ToggleVSync();

	bool isFullScreen()const;
	void SetFullScreen(bool fullScreen);
	void ToggleFullScreen();

	void Show();
	void Hide();

	UINT Present();

protected:
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend class Application;
	friend class Game;

	Window() = delete;
	Window(HWND hWnd, const std::wstring& windowName, int clientWidth, int clientHeight, bool vSync);
	virtual ~Window();

	void UpdateRenderTargetViews();

	Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain();

	void RegisterCallbacks(std::shared_ptr<Game> pGame);

	virtual void OnUpdate(UpdateEventArgs& e);

	virtual void OnRender(RenderEventArgs& e);

	virtual void OnResize(ResizeEventArgs& e);
	
	virtual void OnKeyPressed(KeyEventArgs& e);

	virtual void OnKeyReleased(KeyEventArgs& e);

	virtual void OnMouseWheel(MouseWheelEventArgs& e);

	virtual void OnMouseMoved(MouseMotionEventArgs& e);

	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);

	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);

private:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	HWND m_hWnd;
	std::wstring m_WindowName;
	RECT m_WindowRect;

	int m_ClientWidth;
	int m_ClientHeight;
	bool m_VSync;
	bool m_FullScreen;

	HighResolutionClock m_UpdateClock;
	HighResolutionClock m_RenderClock;
	uint64_t m_FrameCounter;

	std::weak_ptr<Game> m_pGame;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_d3d12BackBuffers[BufferCount];
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_d3d12RTVDescriptorHeap;

	UINT m_RTVDescriptorSize;
	UINT m_CurrentBackBufferIndex;

	bool m_IsTearingSupported;
};