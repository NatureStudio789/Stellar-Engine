#include <Core.h>
#include "../../CommandList/CommandList.h"
#include "VertexBuffer.h"

namespace SE
{
	GVertexBuffer::GVertexBuffer() : GBuffer()
	{

	}

	GVertexBuffer::GVertexBuffer(void* bufferData, 
		unsigned int dataSize, unsigned int dataStride) : GBuffer(bufferData, dataSize, dataStride)
	{

	}

	GVertexBuffer::GVertexBuffer(const GVertexBuffer& other) : GBuffer(other)
	{

	}

	void GVertexBuffer::Apply()
	{
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
		STELLAR_CLEAR_MEMORY(VertexBufferView);
		VertexBufferView.BufferLocation = this->GPUBuffer->GetGPUVirtualAddress();
		VertexBufferView.SizeInBytes = this->DataSize * this->DataStride;
		VertexBufferView.StrideInBytes = this->DataStride;

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			IASetVertexBuffers(1, 1, &VertexBufferView);
	}

	const unsigned int& GVertexBuffer::GetVerticesCount() const noexcept
	{
		return this->DataSize;
	}
}
