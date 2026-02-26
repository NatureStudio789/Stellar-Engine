#include <Core.h>
#include "../Renderer/ResourcePackage/ResourcePackage.h"
#include "../Context/GraphicsContext.h"
#include "Framebuffer.h"

namespace SE
{
	GFramebuffer::GFramebuffer() : SAddressable()
	{
		this->Size = {};
		this->IsPresentingFramebuffer = false;
		this->RTBufferSwapChain = null;
	}

	GFramebuffer::GFramebuffer(const glm::uvec2& size)
	{
		this->Initialize(size);
	}

	GFramebuffer::GFramebuffer(std::shared_ptr<GSwapChain> bufferSwapChain)
	{
		this->Initialize(bufferSwapChain);
	}

	GFramebuffer::GFramebuffer(const GFramebuffer& other) : SAddressable(other)
	{
		this->Size = other.Size;
	}

	GFramebuffer::~GFramebuffer()
	{
	}

	void GFramebuffer::Initialize(const glm::uvec2& size)
	{
		this->IsPresentingFramebuffer = false;

		this->Size = size;

		this->RTVDescriptorHandle = this->GetContext()->GetRTVDescriptorHeap()->Allocate();

		D3D12_CLEAR_VALUE ClearValue;
		STELLAR_CLEAR_MEMORY(ClearValue);
		ClearValue.Color[0] = 0.1f;
		ClearValue.Color[1] = 0.1f;
		ClearValue.Color[2] = 0.1f;
		ClearValue.Color[3] = 1.0f;

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
					this->Size.x, this->Size.y, 1, 1, 1, 0,
					D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				&ClearValue, __uuidof(ID3D12Resource), (void**)this->RenderTargetBuffer.GetAddressOf()));

		this->GetDeviceInstance()->CreateRenderTargetView(
			this->RenderTargetBuffer.Get(), null, this->RTVDescriptorHandle->CPUHandle);

		this->DSVDescriptorHandle = this->GetContext()->GetDSVDescriptorHeap()->Allocate();

		D3D12_RESOURCE_DESC DepthStencilBufferDesc;
		STELLAR_CLEAR_MEMORY(DepthStencilBufferDesc);

		DepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilBufferDesc.Alignment = 0;
		DepthStencilBufferDesc.Width = this->Size.x;
		DepthStencilBufferDesc.Height = this->Size.y;
		DepthStencilBufferDesc.DepthOrArraySize = 1;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE OptimizedClear;
		STELLAR_CLEAR_MEMORY(OptimizedClear);
		OptimizedClear.Format = DepthStencilBufferDesc.Format;
		OptimizedClear.DepthStencil.Depth = 1.0f;
		OptimizedClear.DepthStencil.Stencil = 0;

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
				&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBuffer.GetAddressOf()));

		this->GetDeviceInstance()->CreateDepthStencilView(
			this->DepthStencilBuffer.Get(), null, this->DSVDescriptorHandle->CPUHandle);

		this->GetInitializationCommandListInstance()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		this->GetContext()->ExecuteInitialization();

		this->ViewportInstance.Width = (float)this->Size.x;
		this->ViewportInstance.Height = (float)this->Size.y;
		this->ViewportInstance.TopLeftX = 0.0f;
		this->ViewportInstance.TopLeftY = 0.0f;
		this->ViewportInstance.MinDepth = 0.0f;
		this->ViewportInstance.MaxDepth = 1.0f;

		this->ViewportScissorRect.left = 0;
		this->ViewportScissorRect.right = (long)this->Size.x;
		this->ViewportScissorRect.top = 0;
		this->ViewportScissorRect.bottom = (long)this->Size.y;

		this->Activate();
	}

	void GFramebuffer::Initialize(std::shared_ptr<GSwapChain> bufferSwapChain)
	{
		this->IsPresentingFramebuffer = true;
		this->RTBufferSwapChain = bufferSwapChain;

		this->Size = this->RTBufferSwapChain->GetPresentBuffer()->Size;

		this->RTVDescriptorHandle = this->GetContext()->GetRTVDescriptorHeap()->
			Allocate(this->RTBufferSwapChain->GetPresentBuffer()->Count);

		auto RTV = this->RTVDescriptorHandle->CPUHandle;
		for (UINT i = 0; i < this->RTBufferSwapChain->GetPresentBuffer()->Count; i++)
		{
			this->GetDeviceInstance()->CreateRenderTargetView(
				this->RTBufferSwapChain->GetPresentBuffer()->BufferList[i].Get(), null, RTV);
			RTV.Offset(1, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());
		}

		this->DSVDescriptorHandle = this->GetContext()->GetDSVDescriptorHeap()->Allocate();

		D3D12_RESOURCE_DESC DepthStencilBufferDesc;
		STELLAR_CLEAR_MEMORY(DepthStencilBufferDesc);

		DepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilBufferDesc.Alignment = 0;
		DepthStencilBufferDesc.Width = this->Size.x;
		DepthStencilBufferDesc.Height = this->Size.y;
		DepthStencilBufferDesc.DepthOrArraySize = 1;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE OptimizedClear;
		STELLAR_CLEAR_MEMORY(OptimizedClear);
		OptimizedClear.Format = DepthStencilBufferDesc.Format;
		OptimizedClear.DepthStencil.Depth = 1.0f;
		OptimizedClear.DepthStencil.Stencil = 0;

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
				&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBuffer.GetAddressOf()));

		this->GetDeviceInstance()->CreateDepthStencilView(
			this->DepthStencilBuffer.Get(), null, this->DSVDescriptorHandle->CPUHandle);

		this->GetInitializationCommandListInstance()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		this->GetContext()->ExecuteInitialization();

		this->ViewportInstance.Width = (float)this->Size.x;
		this->ViewportInstance.Height = (float)this->Size.y;
		this->ViewportInstance.TopLeftX = 0.0f;
		this->ViewportInstance.TopLeftY = 0.0f;
		this->ViewportInstance.MinDepth = 0.0f;
		this->ViewportInstance.MaxDepth = 1.0f;

		this->ViewportScissorRect.left = 0;
		this->ViewportScissorRect.right = (long)this->Size.x;
		this->ViewportScissorRect.top = 0;
		this->ViewportScissorRect.bottom = (long)this->Size.y;

		this->Activate();
	}

	void GFramebuffer::Clear(const glm::vec4& color)
	{
		float fcolor[4] = { color.r, color.g, color.b, color.a };
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearRenderTargetView(this->GetRTVDescriptorHandleInstance(), fcolor, 0, null);

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearDepthStencilView(this->DSVDescriptorHandle->CPUHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	void GFramebuffer::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetScissorRects(1, &this->ViewportScissorRect);
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetViewports(1, &this->ViewportInstance);

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			OMSetRenderTargets(1, &this->GetRTVDescriptorHandleInstance(), true, &this->DSVDescriptorHandle->CPUHandle);
	}

	void GFramebuffer::Resize(const glm::uvec2& newSize)
	{
		this->Size = newSize;

		this->GetContext()->Flush();

		if (this->IsPresentingFramebuffer)
		{	
			this->RTBufferSwapChain->Resize(this->Size);

			auto RTV = this->RTVDescriptorHandle->CPUHandle;
			for (UINT i = 0; i < this->RTBufferSwapChain->GetPresentBuffer()->Count; i++)
			{
				this->GetDeviceInstance()->CreateRenderTargetView(
					this->RTBufferSwapChain->GetPresentBuffer()->BufferList[i].Get(), null, RTV);
				RTV.Offset(1, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());
			}
		}
		else
		{
			if (this->RenderTargetBuffer)
			{
				this->RenderTargetBuffer.Reset();
			}

			D3D12_CLEAR_VALUE ClearValue;
			STELLAR_CLEAR_MEMORY(ClearValue);
			ClearValue.Color[0] = 0.1f;
			ClearValue.Color[1] = 0.1f;
			ClearValue.Color[2] = 0.1f;
			ClearValue.Color[3] = 1.0f;

			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
						this->Size.x, this->Size.y, 1, 1, 1, 0,
						D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
					&ClearValue, __uuidof(ID3D12Resource), (void**)this->RenderTargetBuffer.GetAddressOf()));

			this->GetDeviceInstance()->CreateRenderTargetView(
				this->RenderTargetBuffer.Get(), null, this->RTVDescriptorHandle->CPUHandle);
		}

		if (this->DepthStencilBuffer)
		{
			this->DepthStencilBuffer.Reset();
		}

		D3D12_RESOURCE_DESC DepthStencilBufferDesc;
		STELLAR_CLEAR_MEMORY(DepthStencilBufferDesc);

		DepthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthStencilBufferDesc.Alignment = 0;
		DepthStencilBufferDesc.Width = this->Size.x;
		DepthStencilBufferDesc.Height = this->Size.y;
		DepthStencilBufferDesc.DepthOrArraySize = 1;
		DepthStencilBufferDesc.MipLevels = 1;
		DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilBufferDesc.SampleDesc.Count = 1;
		DepthStencilBufferDesc.SampleDesc.Quality = 0;
		DepthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE OptimizedClear;
		STELLAR_CLEAR_MEMORY(OptimizedClear);
		OptimizedClear.Format = DepthStencilBufferDesc.Format;
		OptimizedClear.DepthStencil.Depth = 1.0f;
		OptimizedClear.DepthStencil.Stencil = 0;

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
				&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
				&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBuffer.GetAddressOf()));

		this->GetDeviceInstance()->CreateDepthStencilView(
			this->DepthStencilBuffer.Get(), null, this->DSVDescriptorHandle->CPUHandle);

		this->GetInitializationCommandListInstance()->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		this->GetContext()->ExecuteInitialization();

		this->GetContext()->Flush();

		this->ViewportInstance.Width = (float)this->Size.x;
		this->ViewportInstance.Height = (float)this->Size.y;
		this->ViewportInstance.TopLeftX = 0.0f;
		this->ViewportInstance.TopLeftY = 0.0f;
		this->ViewportInstance.MinDepth = 0.0f;
		this->ViewportInstance.MaxDepth = 1.0f;

		this->ViewportScissorRect.left = 0;
		this->ViewportScissorRect.right = (long)this->Size.x;
		this->ViewportScissorRect.top = 0;
		this->ViewportScissorRect.bottom = (long)this->Size.y;
	}

	void GFramebuffer::Begin()
	{
		if (this->IsPresentingFramebuffer)
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(this->RTBufferSwapChain->GetPresentBuffer()->GetCurrentBuffer().Get(),
					D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		}
		else
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(this->RenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
					D3D12_RESOURCE_STATE_RENDER_TARGET));
		}
	}

	void GFramebuffer::End()
	{
		if (this->IsPresentingFramebuffer)
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(this->RTBufferSwapChain->GetPresentBuffer()->GetCurrentBuffer().Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		}
		else
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(this->RenderTargetBuffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
		}
	}

	GResourcePackage GFramebuffer::GetResourcePackage() const noexcept
	{
		return GResourcePackage{ this->IdentifierHandle, GResourcePackage::SE_RESOURCE_FRAMEBUFFER };
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GFramebuffer::GetRTVDescriptorHandleInstance()
	{
		if (this->IsPresentingFramebuffer)
		{
			auto Handle = this->RTVDescriptorHandle->CPUHandle;
			Handle.Offset(this->RTBufferSwapChain->GetPresentBuffer()->CurrentBufferIndex, 
				this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());

			return Handle;
		}
		else
		{
			return this->RTVDescriptorHandle->CPUHandle;
		}
	}
}
