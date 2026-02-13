#include <Core.h>
#include "GraphicsFactory.h"

namespace SE
{
	GGraphicsFactory::GGraphicsFactory()
	{

	}

	GGraphicsFactory::GGraphicsFactory(std::shared_ptr<FWindow::Handle> windowHandle)
	{
		this->Initialize(windowHandle);
	}

	GGraphicsFactory::GGraphicsFactory(const GGraphicsFactory& other)
	{
		this->FactoryInstance = other.FactoryInstance;
	}

	GGraphicsFactory::~GGraphicsFactory()
	{

	}

	void GGraphicsFactory::Initialize(std::shared_ptr<FWindow::Handle> windowHandle)
	{
		SMessageHandler::Instance->Check(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)this->FactoryInstance.GetAddressOf()));
	}

	WRL::ComPtr<IDXGIFactory4> GGraphicsFactory::GetFactoryInstance()
	{
		return this->FactoryInstance;
	}
}
