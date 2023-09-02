#pragma once

#include "Game.h"
#include "Window.h"
#include <DirectXMath.h>

class Cube :public Game {
public:
	using super = Game;
	Cube(const std::wstring& name, int width, int height, bool vSync = false);

	virtual bool LoadContent() override;

	virtual void UnloadContent() override;

protected:
	virtual void OnUpdate(UpdateEventArgs& e) override;

	virtual void OnRender(RenderEventArgs& e) override;

	virtual void OnKeyPressed(KeyEventArgs& e) override;

	//rtual void OnKeyReleased(KeyEventArgs& e) override;

	//virtual void OnMouseMoved(MouseMotionEventArgs& e) override;

	//virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) override;

	//virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

	virtual void OnMouseWheel(MouseWheelEventArgs& e) override;

	virtual void OnResize(ResizeEventArgs& e) override;

	//virtual void OnWindowDestroy() override;

private:
	void TranslationResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	void ClearRTV(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor);

	void ClearDepth(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, FLOAT depth = 1.0f);

	void UpdateBufferResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, ID3D12Resource** pDestinationResource, ID3D12Resource** pIntermediateResource,
		size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

	void ResizeDepthBuffer(int width, int height);

	uint64_t m_fenceValues[Window::BufferCount] = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_d3d12DSVDescriptorHeap;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PipelineState;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;

	float m_Fov;//存储当前相机的视野，通常用角度表示

	DirectX::XMMATRIX m_ModelMatrix;
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;

	bool m_ContentLoaded;
};

