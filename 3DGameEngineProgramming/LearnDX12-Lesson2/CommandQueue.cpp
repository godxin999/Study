#include "pch.h"
#include "CommandQueue.h"

CommandQueue::CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device,D3D12_COMMAND_LIST_TYPE type):
	m_FenceValue(0),
	m_CommandListType(type),
	m_d3d12Device(device){
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority=D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
	ThrowIfFailed(m_d3d12Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

	m_FenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	assert(m_FenceEvent && "Failed to create fence event handle.");
}

CommandQueue::~CommandQueue(){

}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList(){
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;

	if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue)) {//如果命令分配器队列非空且队首的命令分配器已经达到了栅栏值
		commandAllocator=m_CommandAllocatorQueue.front().commandAllocator;//重用该命令分配器
		m_CommandAllocatorQueue.pop();//从队列中弹出该命令分配器
		ThrowIfFailed(commandAllocator->Reset());//重置命令分配器
	}
	else {
		commandAllocator = CreateCommandAllocator();//否则创建新的命令分配器
	}

	if (!m_CommandListQueue.empty()) {//如果命令列表队列非空
		commandList = m_CommandListQueue.front();//重用该命令列表
		m_CommandListQueue.pop();//从队列中弹出该命令列表

		ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));//重置命令列表
	}
	else {
		commandList = CreateCommandList(commandAllocator);//否则创建新的命令列表
	}
	//将命令分配器与命令列表关联起来，便于在CommandQueue外跟踪命令列表关联的命令分配器
	ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
	return commandList;
}

uint64_t CommandQueue::Signal() {
	uint64_t fenceValueForSignal = ++m_FenceValue;
	ThrowIfFailed(m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValueForSignal));
	return fenceValueForSignal;
}

bool CommandQueue::IsFenceComplete(uint64_t fenceValue){
	return m_d3d12Fence->GetCompletedValue() >= fenceValue;
}

void CommandQueue::WaitForFenceValue(uint64_t fenceValue){
	if (!IsFenceComplete(fenceValue)) {
		m_d3d12Fence->SetEventOnCompletion(fenceValue, m_FenceEvent);
		::WaitForSingleObject(m_FenceEvent, DWORD_MAX);
	}
}

void CommandQueue::Flush(){
	WaitForFenceValue(Signal());
}

uint64_t CommandQueue::ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList){
	commandList->Close();//在执行命令列表前必须先关闭它

	ID3D12CommandAllocator* commandAllocator;
	UINT dataSize = sizeof(commandAllocator);
	ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));//获取命令列表关联的命令分配器，命令分配器的引用计数会增加

	ID3D12CommandList* const ppCommandLists[] = {
		commandList.Get()
	};

	m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
	uint64_t fenceValue = Signal();//产生需要等待的栅栏值，以确保命令分配器可以重复使用

	m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue,commandAllocator });//将命令分配器加入队列
	m_CommandListQueue.push(commandList);//将命令列表加入队列

	commandAllocator->Release();//因为此时命令分配器的所有权已经转移到队列中，所以释放临时COM指针的引用是安全的(减少引用计数)
	return fenceValue;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue::GetD3D12CommandQueue() const{
	return m_d3d12CommandQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator(){
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));
	return commandAllocator;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator){
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
	ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
	return commandList;
}


