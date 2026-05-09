#include <Core.h>
#include "../CommandList/CommandList.h"
#include "IndexBuffer.h"

namespace SE
{
	GIndexBuffer::GIndexBuffer() : GBuffer()
	{

	}

	GIndexBuffer::GIndexBuffer(unsigned int* bufferData, unsigned int dataSize) : 
		GBuffer((void*)bufferData, dataSize, (unsigned int)sizeof(unsigned int))
	{
		
	}

	GIndexBuffer::GIndexBuffer(const GIndexBuffer& other) : GBuffer(other)
	{

	}

	void GIndexBuffer::Apply()
	{
		D3D12_INDEX_BUFFER_VIEW IndexBufferView;
		STELLAR_CLEAR_MEMORY(IndexBufferView);
		IndexBufferView.BufferLocation = this->GPUBuffer->GetGPUVirtualAddress();
		IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		IndexBufferView.SizeInBytes = this->DataSize * this->DataStride;

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->IASetIndexBuffer(&IndexBufferView);
	}

	const unsigned int& GIndexBuffer::GetIndicesCount() const noexcept
	{
		return this->DataSize;
	}
}
