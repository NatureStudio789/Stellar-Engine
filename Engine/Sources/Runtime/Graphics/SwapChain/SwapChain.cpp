#include <Core.h>
#include "../Device/Device.h"
#include "SwapChain.h"

namespace SE
{
	GSwapChain::GSwapChain()
	{
		this->BackBufferSize = {};
		this->BackBufferCount = 0;
		this->CurrentBufferIndex = 0;
		this->BackBufferList.clear();

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
		this->BackBufferSize = other.BackBufferSize;
		this->BackBufferCount = other.BackBufferCount;
		this->CurrentBufferIndex = other.CurrentBufferIndex;
		this->BackBufferList = other.BackBufferList;

		this->Fence = other.Fence;
		this->FenceValue = other.FenceValue;
	}

	GSwapChain::~GSwapChain()
	{

	}

	void GSwapChain::Initialize(std::shared_ptr<GDevice> device, std::shared_ptr<FWindow::Handle> windowHandle,
		const glm::uvec2& backBufferSize, UINT backBufferCount, bool fullscreen)
	{
		this->BackBufferSize = backBufferSize;
		this->BackBufferCount = backBufferCount;
		this->CurrentBufferIndex = 0;

		if (this->BackBufferCount < 2)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Back buffer can't be less than 2!");
		}

		WRL::ComPtr<IDXGIFactory4> Factory;
		SMessageHandler::Instance->Check(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, 
			__uuidof(IDXGIFactory4), (void**)Factory.GetAddressOf()));

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		STELLAR_CLEAR_MEMORY(SwapChainDesc);

		SwapChainDesc.BufferDesc.Width = this->BackBufferSize.x;
		SwapChainDesc.BufferDesc.Height = this->BackBufferSize.y;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		SwapChainDesc.BufferCount = this->BackBufferCount;
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
		this->ReleaseBackBuffer();

		SMessageHandler::Instance->Check(this->SwapChainInstance->ResizeBuffers(
			this->BackBufferCount, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0));

		this->BackBufferSize = newSize;
		this->CurrentBufferIndex = 0;

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
		this->CurrentBufferIndex = (this->CurrentBufferIndex + 1) % this->BackBufferCount;

		this->Flush(device);
	}

	WRL::ComPtr<IDXGISwapChain> GSwapChain::GetInstance()
	{
		return this->SwapChainInstance;
	}

	void GSwapChain::CreateBackBuffer()
	{
		this->BackBufferList.resize(this->BackBufferCount);
		for (UINT i = 0; i < this->BackBufferCount; i++)
		{
			SMessageHandler::Instance->Check(this->SwapChainInstance->GetBuffer(i,
				__uuidof(ID3D12Resource), (void**)this->BackBufferList[i].GetAddressOf()));
		}
	}

	void GSwapChain::ReleaseBackBuffer()
	{
		for (UINT i = 0; i < this->BackBufferCount; i++)
		{
			this->BackBufferList[i].Reset();
		}
		this->BackBufferList.clear();
	}
}
