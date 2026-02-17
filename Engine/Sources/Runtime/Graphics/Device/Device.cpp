#include <Core.h>
#include "Device.h"

namespace SE
{
	GDevice::GDevice()
	{

	}

	GDevice::GDevice(const GDevice& other)
	{
		this->DeviceInstance = other.DeviceInstance;
		this->GraphicsCommandQueue = other.GraphicsCommandQueue;
	}

	GDevice::~GDevice()
	{

	}

	void GDevice::Initialize()
	{
		SMessageHandler::Instance->Check(D3D12CreateDevice(null, D3D_FEATURE_LEVEL_11_0,
			__uuidof(ID3D12Device), (void**)this->DeviceInstance.GetAddressOf()));

		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
		STELLAR_CLEAR_MEMORY(CommandQueueDesc);
		CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		SMessageHandler::Instance->Check(this->DeviceInstance->CreateCommandQueue(
			&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)this->GraphicsCommandQueue.GetAddressOf()));
	}

	UINT GDevice::GetDescriptorSize(GDescriptorHeap::Type type)
	{
		return this->DeviceInstance->GetDescriptorHandleIncrementSize(
			(D3D12_DESCRIPTOR_HEAP_TYPE)type);
	}

	WRL::ComPtr<ID3D12Device> GDevice::GetInstance()
	{
		return this->DeviceInstance;
	}

	WRL::ComPtr<ID3D12CommandQueue> GDevice::GetGraphicsCommandQueue()
	{
		return this->GraphicsCommandQueue;
	}
}
