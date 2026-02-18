#include <Core.h>
#include "../Context/GraphicsContext.h"
#include "Framebuffer.h"

namespace SE
{
	GFramebuffer::GFramebuffer()
	{
		this->Size = {};
		this->IsPresentingFramebuffer = false;
		this->PresentRTBuffer = null;
	}

	GFramebuffer::GFramebuffer(const glm::uvec2& size)
	{
		this->Initialize(size);
	}

	GFramebuffer::GFramebuffer(std::shared_ptr<GPresentBuffer> presentBuffer)
	{
		this->Initialize(presentBuffer);
	}

	GFramebuffer::GFramebuffer(const GFramebuffer& other)
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

		SMessageHandler::Instance->Check(this->GetContext()->GetDevice()->GetInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
					this->Size.x, this->Size.y, 1, 1, 1, 0,
					D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				&ClearValue, __uuidof(ID3D12Resource), (void**)this->RenderTargetBuffer.GetAddressOf()));

		this->GetContext()->GetDevice()->GetInstance()->CreateRenderTargetView(
			this->RenderTargetBuffer.Get(), null, this->RTVDescriptorHandle->CPUHandle);
	}

	void GFramebuffer::Initialize(std::shared_ptr<GPresentBuffer> presentBuffer)
	{
		this->IsPresentingFramebuffer = true;
		this->PresentRTBuffer = presentBuffer;

		this->Size = this->PresentRTBuffer->Size;

		this->RTVDescriptorHandle = this->GetContext()->GetRTVDescriptorHeap()->
			Allocate(this->PresentRTBuffer->Count);

		auto RTV = this->RTVDescriptorHandle->CPUHandle;
		for (UINT i = 0; i < this->PresentRTBuffer->Count; i++)
		{
			this->GetContext()->GetDevice()->GetInstance()->CreateRenderTargetView(
				this->PresentRTBuffer->BufferList[i].Get(), null, RTV);
			RTV.Offset(1, this->GetContext()->GetRTVDescriptorHeap()->GetIncrementSize());
		}
	}

	void GFramebuffer::Clear(const glm::vec4& color)
	{
		float fcolor[4] = { color.r, color.g, color.b, color.a };
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			ClearRenderTargetView(this->RTVDescriptorHandle->CPUHandle, fcolor, 0, null);
	}

	void GFramebuffer::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			OMSetRenderTargets(1, &this->RTVDescriptorHandle->CPUHandle, true, null);
	}
}
