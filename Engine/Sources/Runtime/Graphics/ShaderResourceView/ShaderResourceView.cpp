#include <Core.h>
#include "../Context/GraphicsContext.h"
#include "../PipelineState/PipelineState.h"
#include "ShaderResourceView.h"

namespace SE
{
	GShaderResourceView::GShaderResourceView() : GApplicable()
	{
		this->DescriptorCount = 0;
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
		this->DescriptorCount = other.DescriptorCount;
		this->RootParameterIndex = other.RootParameterIndex;
		this->IsAvailableForApplying = other.IsAvailableForApplying;

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
		this->DescriptorCount = descriptorCount;
		this->DescriptorHandle = this->GetContext()->GetSRVDescriptorHeap()->Allocate(this->DescriptorCount);
		this->IsDescriptorAllocated = true;
	}

	void GShaderResourceView::SetRootParameterIndex(unsigned int rootParameterIndex)
	{
		this->RootParameterIndex = rootParameterIndex;

		this->IsAvailableForApplying = true;
	}

	void GShaderResourceView::SetRootParameterIndex(unsigned int shaderRegisterIndex, const std::string& renderGroup)
	{
		this->RootParameterIndex = SPipelineStateRegistry::GetInstance(renderGroup)->
			GetRootSignature()->GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, shaderRegisterIndex, this->DescriptorCount));

		this->IsAvailableForApplying = true;
	}

	void GShaderResourceView::Apply()
	{
		if (IsAvailableForApplying)
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->
				SetGraphicsRootDescriptorTable(this->RootParameterIndex, this->DescriptorHandle->GPUHandle);
		}
	}

	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GShaderResourceView::GetGPUDescriptor() const noexcept
	{
		return this->DescriptorHandle->GPUHandle;
	}

	const unsigned int& GShaderResourceView::GetRootParameterIndex() const noexcept
	{
		return this->RootParameterIndex;
	}

	bool GShaderResourceView::GetAvailableForApplying() const noexcept
	{
		return this->IsAvailableForApplying;
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
