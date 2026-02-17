#ifndef _SE_DESCRIPTORHANDLE_H_
#define _SE_DESCRIPTORHANDLE_H_
#include "../../Core/Creatable/Creatable.h"

namespace SE
{
	class GDescriptorHandle : public SCreatable<GDescriptorHandle>
	{
	public:
		GDescriptorHandle();
		GDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle,
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
		GDescriptorHandle(const GDescriptorHandle& other);
		~GDescriptorHandle() = default;

		CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle;
		CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle;
	};
}

#endif