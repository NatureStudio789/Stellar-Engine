#include <Core.h>
#include "../Device/Device.h"
#include "CommandList.h"

namespace SE
{
	GCommandList::GCommandList()
	{

	}

	GCommandList::GCommandList(std::shared_ptr<GDevice> device, Type type)
	{
		this->Initialize(device, type);
	}

	GCommandList::GCommandList(const GCommandList& other)
	{
		this->CommandListType = other.CommandListType;

		this->CommandAllocator = other.CommandAllocator;
		this->CommandListInstance = other.CommandListInstance;
	}

	GCommandList::~GCommandList()
	{

	}

	void GCommandList::Initialize(std::shared_ptr<GDevice> device, Type type)
	{
		this->CommandListType = type;

		SMessageHandler::Instance->Check(device->GetInstance()->CreateCommandAllocator(
			(D3D12_COMMAND_LIST_TYPE)this->CommandListType, __uuidof(ID3D12CommandAllocator),
			(void**)this->CommandAllocator.GetAddressOf()));

		SMessageHandler::Instance->Check(device->GetInstance()->CreateCommandList(0,
			(D3D12_COMMAND_LIST_TYPE)this->CommandListType, this->CommandAllocator.Get(),
			null, __uuidof(ID3D12GraphicsCommandList), (void**)this->CommandListInstance.GetAddressOf()));

		SMessageHandler::Instance->Check(this->CommandListInstance->Close());
	}

	void GCommandList::Open()
	{
		SMessageHandler::Instance->Check(this->CommandAllocator->Reset());
		SMessageHandler::Instance->Check(this->CommandListInstance->Reset(
			this->CommandAllocator.Get(), null));
	}

	void GCommandList::Close()
	{
		SMessageHandler::Instance->Check(this->CommandListInstance->Close());
	}

	WRL::ComPtr<ID3D12CommandList> GCommandList::GetInstance()
	{
		return this->CommandListInstance;
	}
}
