#include <Core.h>
#include "PresentBuffer.h"

namespace SE
{
	GPresentBuffer::GPresentBuffer()
	{
		this->Size = {};
		this->Count = 0;
		this->BufferList.clear();

		this->CurrentBufferIndex = 0;
	}

	GPresentBuffer::GPresentBuffer(const GPresentBuffer& other)
	{
		this->Size = other.Size;
		this->Count = other.Count;
		this->BufferList = other.BufferList;
		this->CurrentBufferIndex = other.CurrentBufferIndex;
	}

	GPresentBuffer::~GPresentBuffer()
	{
		this->ResetBuffer();
		this->Count = 0;
	}

	void GPresentBuffer::ResetBuffer()
	{
		this->Size = {};
		
		for (auto& buffer : this->BufferList)
		{
			buffer.Reset();
		}
		this->BufferList.clear();

		this->CurrentBufferIndex = 0;
	}

	WRL::ComPtr<ID3D12Resource> GPresentBuffer::GetCurrentBuffer()
	{
		return this->BufferList[this->CurrentBufferIndex];
	}
}
