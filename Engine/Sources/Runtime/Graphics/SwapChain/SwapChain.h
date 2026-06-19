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
		static constexpr UINT FRAME_COUNT = 2;

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

		void WaitForFrameFence();
		void MoveToNextFrame();

		WRL::ComPtr<IDXGISwapChain> GetInstance();
		std::shared_ptr<GPresentBuffer> GetPresentBuffer();

	private:
		void CreateBackBuffer();

		WRL::ComPtr<IDXGISwapChain> SwapChainInstance;
		std::shared_ptr<GPresentBuffer> PresentBuffer;

		WRL::ComPtr<ID3D12Fence> Fence;
		UINT FenceValue;

		WRL::ComPtr<ID3D12Fence> FrameFences[FRAME_COUNT];
		UINT64 FrameFenceValues[FRAME_COUNT] = {};
		UINT CurrentFrameIndex = 0;
	};
}

#endif