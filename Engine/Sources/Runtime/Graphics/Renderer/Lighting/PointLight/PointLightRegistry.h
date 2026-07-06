#ifndef _SE_POINTLIGHTREGISTRY_H_
#define _SE_POINTLIGHTREGISTRY_H_
#include "PointLight.h"
#include "../ConstantBuffer/LightConstantBuffer.h"

#include "../../../../Core/Registry/InstantiableRegistry.h"

namespace SE
{
	class GPointLightRegistry : public SInstantiableRegistry<GPointLight>
	{
	public:
		GPointLightRegistry();
		GPointLightRegistry(const GPointLightRegistry& other);
		~GPointLightRegistry() override;

		void Register(std::shared_ptr<GPointLight> instance) override;
		void Unregister(std::shared_ptr<GPointLight> instance) override;
		void Unregister(std::string name) override;
		void Unregister(SUUID uuid) override;

	private:
		void UpdateLightData();

		std::shared_ptr<GPointLightCBuffer> LightCBuffer;

		friend class GDeferredRenderer;
	};
}

#endif