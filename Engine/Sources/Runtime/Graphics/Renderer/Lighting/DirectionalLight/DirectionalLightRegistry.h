#ifndef _SE_DIRECTIONALLIGHTREGISTRY_H_
#define _SE_DIRECTIONALLIGHTREGISTRY_H_
#include "../../../../Core/Registry/InstantiableRegistry.h"

#include "DirectionalLight.h"
#include "../ConstantBuffer/LightConstantBuffer.h"

namespace SE
{
	class GDirectionalLightRegistry : public SInstantiableRegistry<GDirectionalLight>
	{
	public:
		GDirectionalLightRegistry();
		GDirectionalLightRegistry(const GDirectionalLightRegistry& other);
		~GDirectionalLightRegistry() override;

		void Register(std::shared_ptr<GDirectionalLight> instance) override;
		void Unregister(std::shared_ptr<GDirectionalLight> instance) override;
		void Unregister(std::string name) override;
		void Unregister(SUUID uuid) override;

	private:
		void UpdateLightData();

		std::shared_ptr<GDirectionalLightCBuffer> LightCBuffer;

		friend class GDeferredRenderer;
	};
}

#endif