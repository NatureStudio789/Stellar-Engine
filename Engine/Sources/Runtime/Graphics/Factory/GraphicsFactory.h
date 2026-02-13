#ifndef _SE_GRAPHICSFACTORY_H_
#define _SE_GRAPHICSFACTORY_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/MessageHandler/MessageHandler.h"

namespace SE
{
	class GGraphicsFactory : public SNoParameterCreatable<GGraphicsFactory>
	{
	public:
		GGraphicsFactory();
		GGraphicsFactory(const GGraphicsFactory& other);
		~GGraphicsFactory();

		void Initialize();

		WRL::ComPtr<IDXGIFactory4> GetInstance();

	private:
		WRL::ComPtr<IDXGIFactory4> FactoryInstance;
	};
}

#endif