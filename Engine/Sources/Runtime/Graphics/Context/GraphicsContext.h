#ifndef _SE_GRAPHICSCONTEXT_H_
#define _SE_GRAPHICSCONTEXT_H_
#include "../Device/Device.h"
#include "../SwapChain/SwapChain.h"
#include "../CommandList/CommandList.h"
#include "../../Function/Application/Window/Window.h"

namespace SE
{
	class GGraphicsContext : public SCreatable<GGraphicsContext>
	{
	public:
		GGraphicsContext();
		GGraphicsContext(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize);
		GGraphicsContext(const GGraphicsContext& other);
		~GGraphicsContext();

		void Initialize(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize);

		void Resize(const glm::uvec2& newSize);
		void Present(UINT syncInterval);
		void Flush();

		void ExecuteCommandLists(const std::vector<ID3D12GraphicsCommandList*>& graphicsCommandLists);
		void ExecuteInitialization();

		std::shared_ptr<GCommandList> GetInitializationCommandList();

		std::shared_ptr<GDevice> GetDevice();
		std::shared_ptr<GSwapChain> GetSwapChain();

		std::shared_ptr<GDescriptorHeap> GetRTVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetDSVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetSRVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetSamplerDescriptorHeap();

	private:
#ifdef SE_DEBUG
		WRL::ComPtr<ID3D12Debug> Debug;
#endif

		std::shared_ptr<GDevice> Device;
		std::shared_ptr<GSwapChain> SwapChain;

		std::shared_ptr<GCommandList> InitializationCommandList;

		std::shared_ptr<GDescriptorHeap> RTVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> DSVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> SRVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> SamplerDescriptorHeap;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GGraphicsContext, GraphicsContextRegistry);
}

#endif