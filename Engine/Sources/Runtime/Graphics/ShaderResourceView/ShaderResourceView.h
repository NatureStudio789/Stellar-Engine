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

		virtual void Apply() override;

		const unsigned int& GetRootParameterIndex() const noexcept;
		std::shared_ptr<GDescriptorHandle> GetDescriptorHandle();
		const bool& GetAllocated() const noexcept;

	private:
		unsigned int RootParameterIndex;
		std::shared_ptr<GDescriptorHandle> DescriptorHandle;
		bool IsDescriptorAllocated;
	};
}

#endif