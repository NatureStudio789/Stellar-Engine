#include <Core.h>
#include "GraphicsContext.h"

namespace SE
{
	GGraphicsContext::GGraphicsContext()
	{
		this->GraphicsFactory = null;
		this->Device = null;
	}

	GGraphicsContext::GGraphicsContext(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize)
	{
		this->Initialize(windowHandle, backBufferSize);
	}

	GGraphicsContext::GGraphicsContext(const GGraphicsContext& other)
	{
		this->GraphicsFactory = other.GraphicsFactory;
		this->Device = other.Device;
	}

	GGraphicsContext::~GGraphicsContext()
	{
	}

	void GGraphicsContext::Initialize(std::shared_ptr<FWindow::Handle> windowHandle, glm::uvec2 backBufferSize)
	{
		this->GraphicsFactory = GGraphicsFactory::Create();

		this->Device = GDevice::Create(this->GraphicsFactory);
	}
}
