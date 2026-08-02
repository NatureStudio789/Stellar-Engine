#include <Core.h>
#include "../Context/GraphicsContext.h"

#include "SRVSet.h"

namespace SE
{
	GSRVSet::GSRVSet() : SAddressable(), GApplicable()
	{
		this->Size = 0;
		this->DescriptorHandle = null;
		this->ResetMemberState();
		this->RootParameterIndex = 0;

		this->IsDescriptorAllocated = false;
	}

	GSRVSet::GSRVSet(const std::string& name, unsigned int size) : SAddressable(), GApplicable()
	{
		this->Initialize(name, size);
	}

	GSRVSet::GSRVSet(const GSRVSet & other) : SAddressable(other), GApplicable(other)
	{
		this->Size = other.Size;

		this->DescriptorHandle = other.DescriptorHandle;
		this->CurrentMemberCount = other.CurrentMemberCount;
		this->RootParameterIndex = other.RootParameterIndex;

		this->IsDescriptorAllocated = other.IsDescriptorAllocated;
	}

	GSRVSet::~GSRVSet()
	{
		this->IsDescriptorAllocated = false;
	}

	void GSRVSet::Initialize(const std::string & name, unsigned int size)
	{
		this->SetName(name);
		this->Size = size;
		this->ResetMemberState();

		this->DescriptorHandle = this->GetContext()->GetSRVDescriptorHeap()->Allocate(this->Size);
		this->IsDescriptorAllocated = true;

		this->Activate();
	}

	void GSRVSet::AppendSRV(GShaderResourceView * srv)
	{
		if (this->CurrentMemberCount >= this->Size)
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("SRV Set named '{}' is full!", this->GetName()));
		}

		this->CurrentMemberCount++;

		unsigned int index = this->CurrentMemberCount - 1;

		srv->GetDescriptorHandle()->CPUHandle = this->DescriptorHandle->CPUHandle;
		srv->GetDescriptorHandle()->CPUHandle.Offset(index, this->GetContext()->GetSRVDescriptorHeap()->GetIncrementSize());
		srv->GetDescriptorHandle()->GPUHandle = this->DescriptorHandle->GPUHandle;
		srv->GetDescriptorHandle()->GPUHandle.Offset(index, this->GetContext()->GetSRVDescriptorHeap()->GetIncrementSize());
		srv->IsDescriptorAllocated = true;
	}

	void GSRVSet::ResetMemberState()
	{
		this->CurrentMemberCount = 0;
	}

	void GSRVSet::SetRootParameterIndex(unsigned int index)
	{
		this->RootParameterIndex = index;
	}

	void GSRVSet::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			SetGraphicsRootDescriptorTable(this->RootParameterIndex, this->DescriptorHandle->GPUHandle);
	}

	unsigned int GSRVSet::GetSize() const noexcept
	{
		return this->Size;
	}

	std::shared_ptr<GDescriptorHandle> GSRVSet::GetDescriptorHandle()
	{
		return this->DescriptorHandle;
	}

	unsigned int GSRVSet::GetRootParameterIndex() const noexcept
	{
		return this->RootParameterIndex;
	}

	GResourcePackage GSRVSet::GetResourcePackage() const noexcept
	{
		return GResourcePackage(this->IdentifierHandle, GResourcePackage::SE_RESOURCE_SRVGROUP);
	}

	bool GSRVSet::GetDescriptorAllocated() const noexcept
	{
		return this->IsDescriptorAllocated;
	}
}
