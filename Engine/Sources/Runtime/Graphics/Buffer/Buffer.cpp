#include <Core.h>
#include "../Context/GraphicsContext.h"
#include "../CommandList/CommandList.h"
#include "Buffer.h"

namespace SE
{
	GBuffer::GBuffer()
	{
		this->BufferData = null;
		this->DataSize = 0;
		this->DataStride = 0;
	}

	GBuffer::GBuffer(void* bufferData, unsigned int dataSize, unsigned int dataStride)
	{
		this->Initialize(bufferData, dataSize, dataStride);
	}

	GBuffer::GBuffer(const GBuffer& other)
	{
		this->CPUBuffer = other.CPUBuffer;
		this->GPUBuffer = other.GPUBuffer;
		this->UploadBuffer = other.UploadBuffer;
		
		this->BufferData = other.BufferData;
		this->DataSize = other.DataSize;
		this->DataStride = other.DataStride;
	}

	GBuffer::~GBuffer()
	{
		this->BufferData = null;
		this->DataSize = 0;
		this->DataStride = 0;
	}

	void GBuffer::Initialize(void* bufferData, unsigned int dataSize, unsigned int dataStride)
	{
		this->BufferData = bufferData;
		this->DataSize = dataSize;
		this->DataStride = dataStride;

		SMessageHandler::Instance->Check(D3DCreateBlob(
			(SIZE_T)(this->DataSize * this->DataStride), this->CPUBuffer.GetAddressOf()));
		CopyMemory(this->CPUBuffer->GetBufferPointer(), this->BufferData, this->DataSize * this->DataStride);

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((UINT64)(this->DataSize * this->DataStride)),
			D3D12_RESOURCE_STATE_COMMON, null, __uuidof(ID3D12Resource), (void**)this->GPUBuffer.GetAddressOf()));

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((UINT64)(this->DataSize * this->DataStride)),
			D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		D3D12_SUBRESOURCE_DATA BufferSubresourceData;
		STELLAR_CLEAR_MEMORY(BufferSubresourceData);
		BufferSubresourceData.pData = this->BufferData;
		BufferSubresourceData.RowPitch = LONG_PTR(this->DataSize * this->DataStride);
		BufferSubresourceData.SlicePitch = BufferSubresourceData.RowPitch;

		this->GetInitializationCommandListInstance()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GPUBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(this->GetInitializationCommandListInstance().Get(),
			this->GPUBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &BufferSubresourceData);
		this->GetInitializationCommandListInstance()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->GPUBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		this->GetContext()->ExecuteInitialization();
	}
}
