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

		std::shared_ptr<GCommandList> GetInitializationCommandList();

	private:
#ifdef SE_DEBUG
		WRL::ComPtr<ID3D12Debug> Debug;
#endif

		std::shared_ptr<GDevice> Device;
		std::shared_ptr<GSwapChain> SwapChain;

		std::shared_ptr<GCommandList> InitializationCommandList;

	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GGraphicsContext, GraphicsContextRegistry);
}

#endif