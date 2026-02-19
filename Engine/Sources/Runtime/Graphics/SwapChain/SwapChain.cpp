#include <Core.h>
#include "../Device/Device.h"
#include "SwapChain.h"

namespace SE
{
	GSwapChain::GSwapChain()
	{
		this->PresentBuffer = null;

		this->FenceValue = 0;
	}

	GSwapChain::GSwapChain(std::shared_ptr<GDevice> device, std::shared_ptr<FWindow::Handle> windowHandle,
		const glm::uvec2& backBufferSize, UINT backBufferCount, bool fullscreen)
	{
		this->Initialize(device, windowHandle, backBufferSize, backBufferCount,fullscreen);
	}

	GSwapChain::GSwapChain(const GSwapChain& other)
	{
		this->SwapChainInstance = other.SwapChainInstance;
		this->PresentBuffer = other.PresentBuffer;

		this->Fence = other.Fence;
		this->FenceValue = other.FenceValue;
	}

	GSwapChain::~GSwapChain()
	{
		
	}

	void GSwapChain::Initialize(std::shared_ptr<GDevice> device, std::shared_ptr<FWindow::Handle> windowHandle,
		const glm::uvec2& backBufferSize, UINT backBufferCount, bool fullscreen)
	{
		this->PresentBuffer = std::make_shared<GPresentBuffer>();

		this->PresentBuffer->Size = backBufferSize;
		this->PresentBuffer->Count = backBufferCount;
		this->PresentBuffer->CurrentBufferIndex = 0;

		if (this->PresentBuffer->Count < 2)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Present buffer can't be less than 2!");
		}

		WRL::ComPtr<IDXGIFactory4> Factory;
		SMessageHandler::Instance->Check(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, 
			__uuidof(IDXGIFactory4), (void**)Factory.GetAddressOf()));

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		STELLAR_CLEAR_MEMORY(SwapChainDesc);

		SwapChainDesc.BufferDesc.Width = this->PresentBuffer->Size.x;
		SwapChainDesc.BufferDesc.Height = this->PresentBuffer->Size.y;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferCount = this->PresentBuffer->Count;
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.OutputWindow = windowHandle->GetWin32Handle();
		SwapChainDesc.Windowed = !fullscreen;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		SMessageHandler::Instance->Check(Factory->CreateSwapChain(device->GetGraphicsCommandQueue().Get(),
			&SwapChainDesc, this->SwapChainInstance.GetAddressOf()));

		this->CreateBackBuffer();

		this->FenceValue = 0;
		SMessageHandler::Instance->Check(device->GetInstance()->CreateFence(
			0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)this->Fence.GetAddressOf()));
	}

	void GSwapChain::Resize(const glm::uvec2& newSize)
	{
		this->PresentBuffer->ResetBuffer();

		SMessageHandler::Instance->Check(this->SwapChainInstance->ResizeBuffers(
			this->PresentBuffer->Count, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0));

		this->PresentBuffer->Size = newSize;

		this->CreateBackBuffer();
	}

	void GSwapChain::Flush(std::shared_ptr<GDevice> device)
	{
		this->FenceValue++;
		SMessageHandler::Instance->Check(
			device->GetGraphicsCommandQueue()->Signal(this->Fence.Get(), this->FenceValue));

		if (this->Fence->GetCompletedValue() < this->FenceValue)
		{
			HANDLE EventHandle = CreateEventExA(null, null, 0, EVENT_ALL_ACCESS);
			if (!EventHandle)
			{
				SMessageHandler::Instance->SetFatal("Graphics", "Failed to create render event");
			}

			SMessageHandler::Instance->Check(this->Fence->SetEventOnCompletion(this->FenceValue, EventHandle));

			WaitForSingleObject(EventHandle, INFINITE);
			CloseHandle(EventHandle);
		}
	}

	void GSwapChain::Present(std::shared_ptr<GDevice> device, UINT syncInterval)
	{
		SMessageHandler::Instance->Check(this->SwapChainInstance->Present(syncInterval, 0));
		this->PresentBuffer->CurrentBufferIndex = 
			(this->PresentBuffer->CurrentBufferIndex + 1) % this->PresentBuffer->Count;

		this->Flush(device);
	}

	WRL::ComPtr<IDXGISwapChain> GSwapChain::GetInstance()
	{
		return this->SwapChainInstance;
	}

	std::shared_ptr<GPresentBuffer> GSwapChain::GetPresentBuffer()
	{
		return this->PresentBuffer;
	}

	void GSwapChain::CreateBackBuffer()
	{
		this->PresentBuffer->BufferList.resize(this->PresentBuffer->Count);
		for (UINT i = 0; i < this->PresentBuffer->Count; i++)
		{
			SMessageHandler::Instance->Check(this->SwapChainInstance->GetBuffer(i,
				__uuidof(ID3D12Resource), (void**)this->PresentBuffer->BufferList[i].GetAddressOf()));
		}
	}
}
