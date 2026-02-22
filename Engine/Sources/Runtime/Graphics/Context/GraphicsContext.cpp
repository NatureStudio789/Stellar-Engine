#include <Core.h>
#include "GraphicsContext.h"

namespace SE
{
	GGraphicsContext::GGraphicsContext() : SAddressable()
	{
		this->Device = null;
	}

	GGraphicsContext::GGraphicsContext(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize)
	{
		this->Initialize(windowHandle, backBufferSize);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other) : SAddressable(other)
	{
		this->Device = other.Device;
	}

	GGraphicsContext::~GGraphicsContext()
	{
	}

	void GGraphicsContext::Initialize(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize)
	{
#ifdef SE_DEBUG
		SMessageHandler::Instance->Check(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)this->Debug.GetAddressOf()));
		this->Debug->EnableDebugLayer();
#endif

		this->Device = GDevice::Create();
		this->SwapChain = GSwapChain::Create(this->Device, windowHandle, backBufferSize, 2, false);

		this->InitializationCommandList = GCommandList::Create(this->Device, GCommandList::SE_COMMAND_LIST_DIRECT);
		this->InitializationCommandList->Open();

		this->RTVDescriptorHeap = GDescriptorHeap::Create(this->Device, 300, GDescriptorHeap::SE_DESCRIPTORHEAP_RTV);
		this->DSVDescriptorHeap = GDescriptorHeap::Create(this->Device, 300, GDescriptorHeap::SE_DESCRIPTORHEAP_DSV);
		this->SRVDescriptorHeap = GDescriptorHeap::Create(this->Device, 4096,
			GDescriptorHeap::SE_DESCRIPTORHEAP_CBVSRVUAV, GDescriptorHeap::SE_DESCRIPTORHEAP_FLAG_SHADERVISIBLE);
		this->SamplerDescriptorHeap = GDescriptorHeap::Create(this->Device, 6,
			GDescriptorHeap::SE_DESCRIPTORHEAP_SAMPLER, GDescriptorHeap::SE_DESCRIPTORHEAP_FLAG_SHADERVISIBLE);

		this->Activate();
	}

	void GGraphicsContext::Resize(const glm::uvec2& newSize)
	{
		this->SwapChain->Resize(newSize);
	}

	void GGraphicsContext::Present(UINT syncInterval)
	{
		this->SwapChain->Present(this->Device, syncInterval);
	}

	void GGraphicsContext::Flush()
	{
		this->SwapChain->Flush(this->Device);
	}

	void GGraphicsContext::ExecuteCommandLists(const std::vector<ID3D12GraphicsCommandList*>& graphicsCommandLists)
	{
		this->Device->ExecuteCommandLists(graphicsCommandLists);

		this->Flush();
	}

	void GGraphicsContext::ExecuteInitialization()
	{
		this->InitializationCommandList->Close();

		std::vector<ID3D12GraphicsCommandList*> CommandLists = 
			{ this->InitializationCommandList->GetInstance().Get() };
		this->Device->ExecuteCommandLists(CommandLists);

		this->Flush();

		this->InitializationCommandList->Open();
	}

	std::shared_ptr<GCommandList> GGraphicsContext::GetInitializationCommandList()
	{
		return this->InitializationCommandList;
	}

	std::shared_ptr<GDevice> GGraphicsContext::GetDevice()
	{
		return this->Device;
	}

	std::shared_ptr<GSwapChain> GGraphicsContext::GetSwapChain()
	{
		return this->SwapChain;
	}
	
	std::shared_ptr<GDescriptorHeap> GGraphicsContext::GetRTVDescriptorHeap()
	{
		return this->RTVDescriptorHeap;
	}
	
	std::shared_ptr<GDescriptorHeap> GGraphicsContext::GetDSVDescriptorHeap()
	{
		return this->DSVDescriptorHeap;
	}
	
	std::shared_ptr<GDescriptorHeap> GGraphicsContext::GetSRVDescriptorHeap()
	{
		return this->SRVDescriptorHeap;
	}

	std::shared_ptr<GDescriptorHeap> GGraphicsContext::GetSamplerDescriptorHeap()
	{
		return this->SamplerDescriptorHeap;
	}
}
