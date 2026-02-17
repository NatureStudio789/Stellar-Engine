#include <Core.h>
#include "../Device/Device.h"
#include "DescriptorHeap.h"

namespace SE
{
	GDescriptorHeap::GDescriptorHeap()
	{
		this->Allocator = null;
		this->IncrementSize = 0;
	}

	GDescriptorHeap::GDescriptorHeap(std::shared_ptr<GDevice> device, 
		unsigned int descriptorCount, Type type, Flag flag)
	{
		this->Initialize(device, descriptorCount, type, flag);
	}

	GDescriptorHeap::GDescriptorHeap(const GDescriptorHeap& other)
	{
		this->DescriptorType = other.DescriptorType;
		this->IncrementSize = other.IncrementSize;

		this->DescriptorHeapInstance = other.DescriptorHeapInstance;
		this->Allocator = other.Allocator;
	}

	GDescriptorHeap::~GDescriptorHeap()
	{
		this->IncrementSize = 0;
	}

	void GDescriptorHeap::Initialize(std::shared_ptr<GDevice> device, 
		unsigned int descriptorCount, Type type, Flag flag)
	{
		this->DescriptorType = type;

		this->Allocator = GDescriptorAllocator::Create(descriptorCount);

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;
		STELLAR_CLEAR_MEMORY(DescriptorHeapDesc);
		DescriptorHeapDesc.NumDescriptors = descriptorCount;
		DescriptorHeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)this->DescriptorType;
		DescriptorHeapDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)flag;
		DescriptorHeapDesc.NodeMask = 0;

		SMessageHandler::Instance->Check(device->GetInstance()->CreateDescriptorHeap(&DescriptorHeapDesc,
			__uuidof(ID3D12DescriptorHeap), (void**)this->DescriptorHeapInstance.GetAddressOf()));

		this->IncrementSize = device->GetDescriptorSize(this->DescriptorType);
	}

	std::shared_ptr<GDescriptorHandle> GDescriptorHeap::Allocate(UINT count)
	{
		unsigned int StartIndex = this->Allocator->Allocate(count);

		CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			this->DescriptorHeapInstance->GetCPUDescriptorHandleForHeapStart());
		CPUHandle.Offset(StartIndex, this->IncrementSize);

		CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle = {};
		if (this->DescriptorHeapInstance->GetDesc().Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		{
			GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(this->DescriptorHeapInstance->GetGPUDescriptorHandleForHeapStart());
			GPUHandle.Offset(StartIndex, this->IncrementSize);
		}

		return GDescriptorHandle::Create(CPUHandle, GPUHandle);
	}

	const GDescriptorHeap::Type& GDescriptorHeap::GetDescriptorType() const noexcept
	{
		return this->DescriptorType;
	}

	WRL::ComPtr<ID3D12DescriptorHeap> GDescriptorHeap::GetInstance()
	{
		return this->DescriptorHeapInstance;
	}

	const UINT& GDescriptorHeap::GetIncrementSize() const noexcept
	{
		return this->IncrementSize;
	}
}
