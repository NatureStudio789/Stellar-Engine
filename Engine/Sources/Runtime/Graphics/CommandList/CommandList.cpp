#include <Core.h>
#include "../Device/Device.h"
#include "CommandList.h"

namespace SE
{
	GCommandList::GCommandList() : SAddressable()
	{

	}

	GCommandList::GCommandList(std::shared_ptr<GDevice> device, Type type)
	{
		this->Initialize(device, type);
	}

	GCommandList::GCommandList(const GCommandList& other) : SAddressable(other)
	{
		this->CommandListType = other.CommandListType;

		for (UINT i = 0; i < ALLOCATOR_COUNT; i++)
		{
			this->CommandAllocators[i] = other.CommandAllocators[i];
		}
		this->CurrentAllocatorIndex = other.CurrentAllocatorIndex;
		this->CommandListInstance = other.CommandListInstance;
	}

	GCommandList::~GCommandList()
	{

	}

	void GCommandList::Initialize(std::shared_ptr<GDevice> device, Type type)
	{
		this->CommandListType = type;
		this->CurrentAllocatorIndex = 0;

		// Create double-buffered command allocators for safe CPU-GPU overlap.
		for (UINT i = 0; i < ALLOCATOR_COUNT; i++)
		{
			SMessageHandler::Instance->Check(device->GetInstance()->CreateCommandAllocator(
				(D3D12_COMMAND_LIST_TYPE)this->CommandListType, __uuidof(ID3D12CommandAllocator),
				(void**)this->CommandAllocators[i].GetAddressOf()));
		}

		SMessageHandler::Instance->Check(device->GetInstance()->CreateCommandList(0,
			(D3D12_COMMAND_LIST_TYPE)this->CommandListType, this->CommandAllocators[0].Get(),
			null, __uuidof(ID3D12GraphicsCommandList), (void**)this->CommandListInstance.GetAddressOf()));

		SMessageHandler::Instance->Check(this->CommandListInstance->Close());

		this->Activate();
	}

	void GCommandList::Open()
	{
		// Cycle to the next allocator — the one that was used 2 frames ago is
		// guaranteed to be finished by the frame pacing fence system.
		this->CurrentAllocatorIndex = (this->CurrentAllocatorIndex + 1) % ALLOCATOR_COUNT;

		SMessageHandler::Instance->Check(this->CommandAllocators[this->CurrentAllocatorIndex]->Reset());
		SMessageHandler::Instance->Check(this->CommandListInstance->Reset(
			this->CommandAllocators[this->CurrentAllocatorIndex].Get(), null));
	}

	void GCommandList::Close()
	{
		SMessageHandler::Instance->Check(this->CommandListInstance->Close());
	}

	WRL::ComPtr<ID3D12GraphicsCommandList> GCommandList::GetInstance()
	{
		return this->CommandListInstance;
	}
}