#include <Core.h>
#include "GraphicsContext.h"

namespace SE
{
	GGraphicsContext::GGraphicsContext()
	{
		this->Device = null;
	}

	GGraphicsContext::GGraphicsContext(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize)
	{
		this->Initialize(windowHandle, backBufferSize);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other)
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
		SCommandListRegistry::Register("Initialization", this->InitializationCommandList);
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

	std::shared_ptr<GCommandList> GGraphicsContext::GetInitializationCommandList()
	{
		return this->InitializationCommandList;
	}
}
