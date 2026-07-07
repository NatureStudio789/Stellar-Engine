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

	GFramebuffer::GFramebuffer(const glm::uvec2& size, unsigned int multipleRenderTargetCount, std::vector<DXGI_FORMAT> renderTargetFormat)
	{
		this->Initialize(size, multipleRenderTargetCount, renderTargetFormat);
	}

	GFramebuffer::GFramebuffer(std::shared_ptr<GSwapChain> bufferSwapChain)
	{
		this->Initialize(bufferSwapChain);
	}

	GFramebuffer::GFramebuffer(const GFramebuffer& other) : SAddressable(other)
	{
		this->Size = other.Size;

		this->IsPresentingFramebuffer = other.IsPresentingFramebuffer;
		this->RTBufferSwapChain = other.RTBufferSwapChain;

		this->RenderTargetBufferList = other.RenderTargetBufferList;
		this->RTShaderResourceViewList = other.RTShaderResourceViewList;
		this->CurrentBufferIndex = other.CurrentBufferIndex;
		this->RTVDescriptorHandle = other.RTVDescriptorHandle;

		this->DepthStencilBufferList = other.DepthStencilBufferList;
		this->DSVDescriptorHandle = other.DSVDescriptorHandle;

		this->ViewportInstance = other.ViewportInstance;
		this->ViewportScissorRect = other.ViewportScissorRect;
	}

	GFramebuffer::~GFramebuffer()
	{
		this->ReleasePendingResources();
	}

	void GFramebuffer::Initialize(const glm::uvec2& size, unsigned int multipleRenderTargetCount, std::vector<DXGI_FORMAT> renderTargetFormat)
	{
		this->IsPresentingFramebuffer = false;

		this->Size = size;
		this->RenderTargetFormat = renderTargetFormat;

		this->RTVDescriptorHandle = this->GetContext()->GetRTVDescriptorHeap()->Allocate(multipleRenderTargetCount);
		this->DSVDescriptorHandle = this->GetContext()->GetDSVDescriptorHeap()->Allocate(multipleRenderTargetCount);
		this->RenderTargetBufferList.resize(multipleRenderTargetCount);
		this->DepthStencilBufferList.resize(multipleRenderTargetCount);
		this->RTShaderResourceViewList.resize(multipleRenderTargetCount);
		this->CurrentBufferIndex = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle = this->RTVDescriptorHandle->CPUHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle = this->DSVDescriptorHandle->CPUHandle;
		for (UINT i = 0; i < multipleRenderTargetCount; i++)
		{
			D3D12_CLEAR_VALUE ClearValue;
			STELLAR_CLEAR_MEMORY(ClearValue);
			ClearValue.Color[0] = 0.0f;
			ClearValue.Color[1] = 0.0f;
			ClearValue.Color[2] = 0.0f;
			ClearValue.Color[3] = 1.0f;
			ClearValue.Format = this->RenderTargetFormat[i];

			{
				auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				auto ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(this->RenderTargetFormat[i],
					this->Size.x, this->Size.y, 1, 1, 1, 0,
					D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
				SMessageHandler::Instance->Check(this->GetDeviceInstance()->
					CreateCommittedResource(&HeapProperties,
						D3D12_HEAP_FLAG_NONE,
						&ResourceDesc,
						D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
						&ClearValue, __uuidof(ID3D12Resource), (void**)this->RenderTargetBufferList[i].GetAddressOf()));
			}

			this->GetDeviceInstance()->CreateRenderTargetView(
				this->RenderTargetBufferList[i].Get(), null, RTVHandle);
			RTVHandle.Offset(1, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());

			this->RTShaderResourceViewList[i] = std::make_shared<GShaderResourceView>(i);
			this->GetDeviceInstance()->CreateShaderResourceView(
				this->RenderTargetBufferList[i].Get(), null, this->RTShaderResourceViewList[i]->GetDescriptorHandle()->CPUHandle);
			
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

			{
				auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				SMessageHandler::Instance->Check(this->GetDeviceInstance()->
					CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
						&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
						&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBufferList[i].GetAddressOf()));
			}

			this->GetDeviceInstance()->CreateDepthStencilView(
				this->DepthStencilBufferList[i].Get(), null, DSVHandle);
			DSVHandle.Offset(1, this->GetContext()->GetDSVDescriptorHeap()->GetIncrementSize());

			{
				auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBufferList[i].Get(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
				this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
			}
		}


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
		this->DepthStencilBufferList.resize(1);

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

		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
					&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBufferList[0].GetAddressOf()));
		}

		this->GetDeviceInstance()->CreateDepthStencilView(
			this->DepthStencilBufferList[0].Get(), null, this->DSVDescriptorHandle->CPUHandle);

		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBufferList[0].Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
		}
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

	void GFramebuffer::Resize(const glm::uvec2& newSize)
	{
		this->Size = newSize;

		this->GetContext()->Flush();

		/*Release any previously pending resources now that the GPU fence guarantees
		all submitted work that might have referenced them is complete.*/
		this->ReleasePendingResources();

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

			/*Move the old depth-stencil resource to the pending-release queue instead of
			calling Reset() immediately so it stays alive until the GPU finishes executing
			the current frame's command lists.*/
			if (this->DepthStencilBufferList[0])
			{
				this->PendingReleaseDS.push_back(std::move(this->DepthStencilBufferList[0]));
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

			{
				auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				SMessageHandler::Instance->Check(this->GetDeviceInstance()->
					CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
						&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
						&OptimizedClear, __uuidof(ID3D12Resource), (void**)this->DepthStencilBufferList[0].GetAddressOf()));
			}

			this->GetDeviceInstance()->CreateDepthStencilView(
				this->DepthStencilBufferList[0].Get(), null, this->DSVDescriptorHandle->CPUHandle);

			{
				auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->DepthStencilBufferList[0].Get(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
				this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
			}
		}
		else
		{

			CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle = this->RTVDescriptorHandle->CPUHandle;
			CD3DX12_CPU_DESCRIPTOR_HANDLE DSVHandle = this->DSVDescriptorHandle->CPUHandle;
			for (UINT i = 0; i < (UINT)this->RenderTargetBufferList.size(); i++)
			{
				D3D12_CLEAR_VALUE ClearValue;
				STELLAR_CLEAR_MEMORY(ClearValue);
				ClearValue.Color[0] = 0.0f;
				ClearValue.Color[1] = 0.0f;
				ClearValue.Color[2] = 0.0f;
				ClearValue.Color[3] = 1.0f;
				ClearValue.Format = this->RenderTargetFormat[i];

				auto& renderTargetBuffer = this->RenderTargetBufferList[i];
				/*Move the old render-target resource to the pending-release queue instead of
				calling Reset() immediately so it stays alive until the GPU finishes executing
				the current frame's command lists (which may have already been recorded to
				reference these resources before the resize was triggered).*/
				if (renderTargetBuffer)
				{
					this->PendingReleaseRT.push_back(std::move(renderTargetBuffer));
				}
				auto& shaderResourceView = this->RTShaderResourceViewList[i];

				{
					auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
					auto ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(this->RenderTargetFormat[i],
						this->Size.x, this->Size.y, 1, 1, 1, 0,
						D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
					SMessageHandler::Instance->Check(this->GetDeviceInstance()->
						CreateCommittedResource(&HeapProperties,
							D3D12_HEAP_FLAG_NONE,
							&ResourceDesc,
							D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
							&ClearValue, __uuidof(ID3D12Resource), (void**)renderTargetBuffer.GetAddressOf()));
				}

				this->GetDeviceInstance()->CreateRenderTargetView(
					renderTargetBuffer.Get(), null, RTVHandle);
				RTVHandle.Offset(1, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());

				this->GetDeviceInstance()->CreateShaderResourceView(
					renderTargetBuffer.Get(), null, shaderResourceView->GetDescriptorHandle()->CPUHandle);

				auto& depthStencilBuffer = this->DepthStencilBufferList[i];
				if (depthStencilBuffer)
				{
					this->PendingReleaseDS.push_back(std::move(depthStencilBuffer));
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

				{
					auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
					SMessageHandler::Instance->Check(this->GetDeviceInstance()->
						CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
							&DepthStencilBufferDesc, D3D12_RESOURCE_STATE_COMMON,
							&OptimizedClear, __uuidof(ID3D12Resource), (void**)depthStencilBuffer.GetAddressOf()));
				}

				this->GetDeviceInstance()->CreateDepthStencilView(depthStencilBuffer.Get(), null, DSVHandle);
				DSVHandle.Offset(1, this->GetContext()->GetDSVDescriptorHeap()->GetIncrementSize());

				{
					auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(depthStencilBuffer.Get(),
						D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
					this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
				}
			}
		}

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

	void GFramebuffer::SetCurrentBuffer(unsigned int multipleRenderTargetBufferIndex)
	{
		if (multipleRenderTargetBufferIndex >= (unsigned int)this->RenderTargetBufferList.size())
		{
			SMessageHandler::Instance->SetFatal("Graphics", "The buffer index of framebuffer is out of its buffer list range!");
		}

		this->CurrentBufferIndex = multipleRenderTargetBufferIndex;
	}

	void GFramebuffer::Begin()
	{
		if (this->IsPresentingFramebuffer)
		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->RTBufferSwapChain->GetPresentBuffer()->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &Barrier);
		}
		else
		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->RenderTargetBufferList[this->CurrentBufferIndex].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &Barrier);
		}
	}

	void GFramebuffer::BeginMultiple(const std::vector<unsigned int>& bufferIndices)
	{
		std::vector<D3D12_RESOURCE_BARRIER> barriers;
		barriers.reserve(bufferIndices.size());
		for (auto index : bufferIndices)
		{
			barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(
				this->RenderTargetBufferList[index].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
		}
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(
			(UINT)barriers.size(), barriers.data());
	}

	void GFramebuffer::EndMultiple(const std::vector<unsigned int>& bufferIndices)
	{
		std::vector<D3D12_RESOURCE_BARRIER> barriers;
		barriers.reserve(bufferIndices.size());
		for (auto index : bufferIndices)
		{
			barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(
				this->RenderTargetBufferList[index].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
		}
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(
			(UINT)barriers.size(), barriers.data());
	}

	void GFramebuffer::Clear(const glm::vec4& color)
	{
		float fcolor[4] = { color.r, color.g, color.b, color.a };
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearRenderTargetView(this->GetRTVDescriptorHandleInstance(), fcolor, 0, null);

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearDepthStencilView(this->GetDSVDescriptorHandleInstance(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, null);
	}

	void GFramebuffer::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetScissorRects(1, &this->ViewportScissorRect);
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->RSSetViewports(1, &this->ViewportInstance);

		auto CurrentRTVDesciptorHandleInstance = this->GetRTVDescriptorHandleInstance();
		auto CurrentDSVDescriptorHandleInstance = this->GetDSVDescriptorHandleInstance();
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			OMSetRenderTargets(1, &CurrentRTVDesciptorHandleInstance, true, &CurrentDSVDescriptorHandleInstance);
	}

	void GFramebuffer::End()
	{
		if (this->IsPresentingFramebuffer)
		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->RTBufferSwapChain->GetPresentBuffer()->GetCurrentBuffer().Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &Barrier);
		}
		else
		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->RenderTargetBufferList[this->CurrentBufferIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->ResourceBarrier(1, &Barrier);
		}
	}

	std::shared_ptr<GShaderResourceView> GFramebuffer::GetRTShaderResourceView(unsigned int multipleRenderTargetBufferIndex)
	{
		if (this->IsPresentingFramebuffer)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "This framebuffer is presenting framebuffer! Cannot get a shader resource view from it!");
		}

		if (multipleRenderTargetBufferIndex >= (unsigned int)this->RenderTargetBufferList.size())
		{
			SMessageHandler::Instance->SetFatal("Graphics", "The buffer index of framebuffer is out of its buffer list range!");
		}

		return this->RTShaderResourceViewList[multipleRenderTargetBufferIndex];
	}

	GResourcePackage GFramebuffer::GetResourcePackage() const noexcept
	{
		return GResourcePackage{ this->IdentifierHandle, GResourcePackage::SE_RESOURCE_FRAMEBUFFER };
	}

	const glm::uvec2& GFramebuffer::GetSize() const noexcept
	{
		return this->Size;
	}

	unsigned int GFramebuffer::GetWidth() const noexcept
	{
		return this->Size.x;
	}

	unsigned int GFramebuffer::GetHeight() const noexcept
	{
		return this->Size.y;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GFramebuffer::GetRTVDescriptorHandleInstance()
	{
		auto Handle = this->RTVDescriptorHandle->CPUHandle;
		if (this->IsPresentingFramebuffer)
		{
			Handle.Offset(this->RTBufferSwapChain->GetPresentBuffer()->CurrentBufferIndex, 
				this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());
		}
		else
		{
			Handle.Offset(this->CurrentBufferIndex, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());
		}

		return Handle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GFramebuffer::GetDSVDescriptorHandleInstance()
	{
		auto Handle = this->DSVDescriptorHandle->CPUHandle;
		if (!this->IsPresentingFramebuffer)
		{
			Handle.Offset(this->CurrentBufferIndex, this->GetContext()->GetDSVDescriptorHeap()->GetIncrementSize());
		}

		return Handle;
	}

	void GFramebuffer::ReleasePendingResources()
	{
		for (auto& RTBuffer : this->PendingReleaseRT)
		{
			RTBuffer.Reset();
		}
		this->PendingReleaseRT.clear();

		for (auto& DSBuffer : this->PendingReleaseDS)
		{
			DSBuffer.Reset();
		}
		this->PendingReleaseDS.clear();
	}
}
