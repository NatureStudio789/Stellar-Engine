#include <Core.h>
#include "DescriptorHandle.h"

namespace SE
{
	GDescriptorHandle::GDescriptorHandle()
	{
		this->CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE();
		this->GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE();
	}

	GDescriptorHandle::GDescriptorHandle(
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		this->CPUHandle = cpuHandle;
		this->GPUHandle = gpuHandle;
	}

	GDescriptorHandle::GDescriptorHandle(const GDescriptorHandle& other)
	{
		this->CPUHandle = other.CPUHandle;
		this->GPUHandle = other.GPUHandle;
	}
}
