#include <Core.h>
#include "../../Context/GraphicsContext.h"

#include "DepthMap.h"

namespace SE
{
	GDepthMap::GDepthMap() : SAddressable()
	{

	}

	GDepthMap::GDepthMap(const glm::uvec2& size) : SAddressable()
	{
		this->Initialize(size);
	}

	GDepthMap::GDepthMap(const GDepthMap & other) : SAddressable(other)
	{
		this->Size = other.Size;

		this->DepthMapBuffer = other.DepthMapBuffer;
		this->DSVDescriptorHandle = other.DSVDescriptorHandle;
		this->DepthMapSRV = other.DepthMapSRV;

		this->ViewportInstance = other.ViewportInstance;
		this->ViewportScissorRect = other.ViewportScissorRect;
	}

	GDepthMap::~GDepthMap()
	{

	}

	void GDepthMap::Initialize(const glm::uvec2 & size)
	{
		this->Size = size;

		D3D12_RESOURCE_DESC	DepthMapBufferDesc;
		STELLAR_CLEAR_MEMORY(DepthMapBufferDesc);

		DepthMapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthMapBufferDesc.Alignment = 0;
		DepthMapBufferDesc.Width = this->Size.x;
		DepthMapBufferDesc.Height = this->Size.y;
		DepthMapBufferDesc.DepthOrArraySize = 1;
		DepthMapBufferDesc.MipLevels = 1;
		DepthMapBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthMapBufferDesc.SampleDesc.Count = 1;
		DepthMapBufferDesc.SampleDesc.Quality = 0;
		DepthMapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthMapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE BufferClearValue;
		STELLAR_CLEAR_MEMORY(BufferClearValue);
		BufferClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		BufferClearValue.DepthStencil.Depth = 1.0f;
		BufferClearValue.DepthStencil.Stencil = 0;

		auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateCommittedResource(&HeapProperties,
				D3D12_HEAP_FLAG_NONE, &DepthMapBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, &BufferClearValue,
				__uuidof(ID3D12Resource), (void**)this->DepthMapBuffer.GetAddressOf()));

		this->DSVDescriptorHandle = this->GetContext()->GetDSVDescriptorHeap()->Allocate(1);
		if (!this->DepthMapSRV)
		{
			this->DepthMapSRV = std::make_shared<GShaderResourceView>(0);
		}
		else
		{
			if (!this->DepthMapSRV->GetAllocated())
			{
				this->DepthMapSRV->AllocateDescriptor();
			}
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC DepthMapDSVDesc;
		STELLAR_CLEAR_MEMORY(DepthMapDSVDesc);
		DepthMapDSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		DepthMapDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthMapDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthMapDSVDesc.Texture2D.MipSlice = 0;
		this->GetDeviceInstance()->CreateDepthStencilView(this->DepthMapBuffer.Get(), &DepthMapDSVDesc, this->DSVDescriptorHandle->CPUHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		STELLAR_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		this->GetDeviceInstance()->CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->DepthMapSRV->GetDescriptorHandle()->CPUHandle);

		this->ViewportInstance.TopLeftX = 0.0f;
		this->ViewportInstance.TopLeftY = 0.0f;
		this->ViewportInstance.Width = (float)this->Size.x;
		this->ViewportInstance.Height = (float)this->Size.y;
		this->ViewportInstance.MinDepth = 0.0f;
		this->ViewportInstance.MaxDepth = 1.0f;
		
		this->ViewportScissorRect.left = 0;
		this->ViewportScissorRect.right = this->Size.x;
		this->ViewportScissorRect.top = 0;
		this->ViewportScissorRect.bottom = this->Size.y;

		this->Activate();
	}

	void GDepthMap::Resize(const glm::uvec2 & newSize)
	{
		this->Size = newSize;

		this->GetContext()->Flush();

		if (this->DepthMapBuffer)
		{
			this->DepthMapBuffer.Reset();
		}

		D3D12_RESOURCE_DESC	DepthMapBufferDesc;
		STELLAR_CLEAR_MEMORY(DepthMapBufferDesc);

		DepthMapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthMapBufferDesc.Alignment = 0;
		DepthMapBufferDesc.Width = this->Size.x;
		DepthMapBufferDesc.Height = this->Size.y;
		DepthMapBufferDesc.DepthOrArraySize = 1;
		DepthMapBufferDesc.MipLevels = 1;
		DepthMapBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthMapBufferDesc.SampleDesc.Count = 1;
		DepthMapBufferDesc.SampleDesc.Quality = 0;
		DepthMapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthMapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE BufferClearValue;
		STELLAR_CLEAR_MEMORY(BufferClearValue);
		BufferClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		BufferClearValue.DepthStencil.Depth = 1.0f;
		BufferClearValue.DepthStencil.Stencil = 0;

		auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateCommittedResource(&HeapProperties,
			D3D12_HEAP_FLAG_NONE, &DepthMapBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, &BufferClearValue,
			__uuidof(ID3D12Resource), (void**)this->DepthMapBuffer.GetAddressOf()));

		D3D12_DEPTH_STENCIL_VIEW_DESC DepthMapDSVDesc;
		STELLAR_CLEAR_MEMORY(DepthMapDSVDesc);
		DepthMapDSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		DepthMapDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		DepthMapDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthMapDSVDesc.Texture2D.MipSlice = 0;
		this->GetDeviceInstance()->CreateDepthStencilView(this->DepthMapBuffer.Get(), &DepthMapDSVDesc, this->DSVDescriptorHandle->CPUHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		STELLAR_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		this->GetDeviceInstance()->CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->DepthMapSRV->GetDescriptorHandle()->CPUHandle);

		this->ViewportInstance.TopLeftX = 0.0f;
		this->ViewportInstance.TopLeftY = 0.0f;
		this->ViewportInstance.Width = (float)this->Size.x;
		this->ViewportInstance.Height = (float)this->Size.y;
		this->ViewportInstance.MinDepth = 0.0f;
		this->ViewportInstance.MaxDepth = 1.0f;

		this->ViewportScissorRect.left = 0;
		this->ViewportScissorRect.right = this->Size.x;
		this->ViewportScissorRect.top = 0;
		this->ViewportScissorRect.bottom = this->Size.y;
	}

	void GDepthMap::Begin()
	{
		CD3DX12_RESOURCE_BARRIER ResourceBarrrier = CD3DX12_RESOURCE_BARRIER::Transition(this->DepthMapBuffer.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &ResourceBarrrier);
	}

	void GDepthMap::Clear()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearDepthStencilView(this->DSVDescriptorHandle->CPUHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	void GDepthMap::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetScissorRects(1, &this->ViewportScissorRect);
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetViewports(1, &this->ViewportInstance);

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->OMSetRenderTargets(0, null, false, &this->DSVDescriptorHandle->CPUHandle);
	}

	void GDepthMap::End()
	{
		CD3DX12_RESOURCE_BARRIER ResourceBarrrier = CD3DX12_RESOURCE_BARRIER::Transition(this->DepthMapBuffer.Get(),
			D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &ResourceBarrrier);
	}

	void GDepthMap::RebindSRV()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC DepthMapSRVDesc;
		STELLAR_CLEAR_MEMORY(DepthMapSRVDesc);
		DepthMapSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		DepthMapSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		DepthMapSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		DepthMapSRVDesc.Texture2D.MostDetailedMip = 0;
		DepthMapSRVDesc.Texture2D.MipLevels = 1;
		DepthMapSRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DepthMapSRVDesc.Texture2D.PlaneSlice = 0;

		this->GetDeviceInstance()->CreateShaderResourceView(this->DepthMapBuffer.Get(), &DepthMapSRVDesc, this->DepthMapSRV->GetDescriptorHandle()->CPUHandle);
	}

	std::shared_ptr<GShaderResourceView> GDepthMap::GetShaderResourceView()
	{
		return this->DepthMapSRV;
	}

	GResourcePackage GDepthMap::GetResourcePackage() const noexcept
	{
		return GResourcePackage(this->IdentifierHandle, GResourcePackage::SE_RESOURCE_DEPTHMAP);
	}

	const glm::uvec2& GDepthMap::GetSize() const noexcept
	{
		return this->Size;
	}

	unsigned int GDepthMap::GetWidth() const noexcept
	{
		return this->Size.x;
	}

	unsigned int GDepthMap::GetHeight() const noexcept
	{
		return this->Size.y;
	}
}
