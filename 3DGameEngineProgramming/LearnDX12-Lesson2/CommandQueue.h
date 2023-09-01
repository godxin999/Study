#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <queue>

class CommandQueue {
public:
	CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	virtual ~CommandQueue();

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue()const;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> GetCommandList();

	uint64_t ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList);

	uint64_t Signal();
	bool IsFenceComplete(uint64_t fenceValue);
	void WaitForFenceValue(uint64_t fenceValue);
	void Flush();

protected:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator);
private:
	//将栅栏值和命令分配器关联起来，方便未来重用
	struct CommandAllocatorEntry {
		uint64_t fenceValue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	};

	using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;//对在GPU队列中运行的命令分配器进行排队
	using CommandListQueue = std::queue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>>;//对可重复使用的命令列表进行排队，由于命令列表在命令队列中执行后可以立刻重复使用，所以不需要和栅栏值关联起来

	D3D12_COMMAND_LIST_TYPE m_CommandListType;//命令列表类型

	Microsoft::WRL::ComPtr<ID3D12Device2> m_d3d12Device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_d3d12CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_d3d12Fence;
	
	HANDLE m_FenceEvent;
	uint64_t m_FenceValue;

	CommandAllocatorQueue m_CommandAllocatorQueue;
	CommandListQueue m_CommandListQueue;
};