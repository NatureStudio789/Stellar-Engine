#ifndef _SE_SHADERRESOURCEVIEW_H_
#define _SE_SHADERRESOURCEVIEW_H_
#include "../Applicable/Applicable.h"
#include "../Descriptor/DescriptorHandle.h"

namespace SE
{
	class GShaderResourceView : public GApplicable
	{
	public:
		GShaderResourceView();
		GShaderResourceView(
			unsigned int rootParameterIndex, unsigned int descriptorCount = 1);
		GShaderResourceView(const GShaderResourceView& other);
		virtual ~GShaderResourceView();

		void AllocateDescriptor(unsigned int descriptorCount = 1);

		void SetRootParameterIndex(unsigned int rootParameterIndex);
		void SetRootParameterIndex(unsigned int shaderRegisterIndex, const std::string& renderGroup);

		virtual void Apply() override;

		const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUDescriptor() const noexcept;
		const unsigned int& GetRootParameterIndex() const noexcept;
		bool GetAvailableForApplying() const noexcept;
		std::shared_ptr<GDescriptorHandle> GetDescriptorHandle();
		const bool& GetAllocated() const noexcept;

	protected:
		unsigned int DescriptorCount;
		unsigned int RootParameterIndex;
		bool IsAvailableForApplying = false;

		std::shared_ptr<GDescriptorHandle> DescriptorHandle;
		bool IsDescriptorAllocated;

		friend class GSRVSet;
	};
}

#endif