#ifndef _SE_GRAPHICSFACTORY_H_
#define _SE_GRAPHICSFACTORY_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/MessageHandler/MessageHandler.h"
#include "../../Function/Application/Window/Window.h"

namespace SE
{
	class GGraphicsFactory : public SCreatable<GGraphicsFactory>
	{
	public:
		GGraphicsFactory();
		GGraphicsFactory(std::shared_ptr<FWindow::Handle> windowHandle);
		GGraphicsFactory(const GGraphicsFactory& other);
		~GGraphicsFactory();

		void Initialize(std::shared_ptr<FWindow::Handle> windowHandle);

		WRL::ComPtr<IDXGIFactory4> GetFactoryInstance();

	private:
		WRL::ComPtr<IDXGIFactory4> FactoryInstance;
	};
}

#endif