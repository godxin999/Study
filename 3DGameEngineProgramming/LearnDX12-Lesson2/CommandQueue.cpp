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

	if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue)) {//���������������зǿ��Ҷ��׵�����������Ѿ��ﵽ��դ��ֵ
		commandAllocator=m_CommandAllocatorQueue.front().commandAllocator;//���ø����������
		m_CommandAllocatorQueue.pop();//�Ӷ����е��������������
		ThrowIfFailed(commandAllocator->Reset());//�������������
	}
	else {
		commandAllocator = CreateCommandAllocator();//���򴴽��µ����������
	}

	if (!m_CommandListQueue.empty()) {//��������б���зǿ�
		commandList = m_CommandListQueue.front();//���ø������б�
		m_CommandListQueue.pop();//�Ӷ����е����������б�

		ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));//���������б�
	}
	else {
		commandList = CreateCommandList(commandAllocator);//���򴴽��µ������б�
	}
	//������������������б����������������CommandQueue����������б���������������
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
	commandList->Close();//��ִ�������б�ǰ�����ȹر���

	ID3D12CommandAllocator* commandAllocator;
	UINT dataSize = sizeof(commandAllocator);
	ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));//��ȡ�����б�������������������������������ü���������

	ID3D12CommandList* const ppCommandLists[] = {
		commandList.Get()
	};

	m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
	uint64_t fenceValue = Signal();//������Ҫ�ȴ���դ��ֵ����ȷ����������������ظ�ʹ��

	m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue,commandAllocator });//������������������
	m_CommandListQueue.push(commandList);//�������б�������

	commandAllocator->Release();//��Ϊ��ʱ���������������Ȩ�Ѿ�ת�Ƶ������У������ͷ���ʱCOMָ��������ǰ�ȫ��(�������ü���)
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


