#include <Core.h>
#include "../Context/GraphicsContext.h"
#include "ShaderResourceView.h"

namespace SE
{
	GShaderResourceView::GShaderResourceView() : GApplicable()
	{
		this->RootParameterIndex = 0;
		this->DescriptorHandle = null;
		this->IsDescriptorAllocated = false;
	}

	GShaderResourceView::GShaderResourceView(
		unsigned int rootParameterIndex, unsigned int descriptorCount) : GApplicable()
	{
		this->SetRootParameterIndex(rootParameterIndex);

		this->AllocateDescriptor(descriptorCount);
	}

	GShaderResourceView::GShaderResourceView(const GShaderResourceView& other) : GApplicable(other)
	{
		this->RootParameterIndex = other.RootParameterIndex;
		this->DescriptorHandle = other.DescriptorHandle;
		this->IsDescriptorAllocated = other.IsDescriptorAllocated;
	}

	GShaderResourceView::~GShaderResourceView()
	{
		this->IsDescriptorAllocated = false;
		this->RootParameterIndex = 0;
	}

	void GShaderResourceView::AllocateDescriptor(unsigned int descriptorCount)
	{
		this->DescriptorHandle = this->GetContext()->GetSRVDescriptorHeap()->Allocate(descriptorCount);
		this->IsDescriptorAllocated = true;
	}

	void GShaderResourceView::SetRootParameterIndex(unsigned int rootParameterIndex)
	{
		this->RootParameterIndex = rootParameterIndex;
	}

	void GShaderResourceView::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			SetGraphicsRootDescriptorTable(this->RootParameterIndex, this->DescriptorHandle->GPUHandle);
	}

	const unsigned int& GShaderResourceView::GetRootParameterIndex() const noexcept
	{
		return this->RootParameterIndex;
	}

	std::shared_ptr<GDescriptorHandle> GShaderResourceView::GetDescriptorHandle()
	{
		return this->DescriptorHandle;
	}

	const bool& GShaderResourceView::GetAllocated() const noexcept
	{
		return this->IsDescriptorAllocated;
	}
}
