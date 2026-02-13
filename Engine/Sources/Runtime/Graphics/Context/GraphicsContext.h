#ifndef _SE_GRAPHICSCONTEXT_H_
#define _SE_GRAPHICSCONTEXT_H_
#include "../Device/Device.h"
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

	private:
		std::shared_ptr<GGraphicsFactory> GraphicsFactory;
		std::shared_ptr<GDevice> Device;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GGraphicsContext, GraphicsContextRegistry);
}

#endif