#ifndef _SE_SWAPCHAIN_H_
#define _SE_SWAPCHAIN_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/MessageHandler/MessageHandler.h"
#include "../../Function/Application/Window/Window.h"
#include "PresentBuffer/PresentBuffer.h"

namespace SE
{
	class GDevice;

	class GSwapChain : public SCreatable<GSwapChain>
	{
	public:
		GSwapChain();
		GSwapChain(std::shared_ptr<GDevice> device, std::shared_ptr<FWindow::Handle> windowHandle, 
			const glm::uvec2& backBufferSize, UINT backBufferCount, bool fullscreen);
		GSwapChain(const GSwapChain& other);
		~GSwapChain();

		void Initialize(std::shared_ptr<GDevice> device, std::shared_ptr<FWindow::Handle> windowHandle,
			const glm::uvec2& backBufferSize, UINT backBufferCount, bool fullscreen);

		void Resize(const glm::uvec2& newSize);

		void Flush(std::shared_ptr<GDevice> device);
		void Present(std::shared_ptr<GDevice> device, UINT syncInterval);

		WRL::ComPtr<IDXGISwapChain> GetInstance();

	private:
		void CreateBackBuffer();

		WRL::ComPtr<IDXGISwapChain> SwapChainInstance;
		std::shared_ptr<GPresentBuffer> PresentBuffer;

		WRL::ComPtr<ID3D12Fence> Fence;
		UINT FenceValue;
	};
}

#endif