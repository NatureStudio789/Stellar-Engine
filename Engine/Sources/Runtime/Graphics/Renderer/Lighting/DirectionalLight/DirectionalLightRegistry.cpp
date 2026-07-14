#include <Core.h>
#include "DirectionalLightRegistry.h"

namespace SE
{
	GDirectionalLightRegistry::GDirectionalLightRegistry() : SInstantiableRegistry<GDirectionalLight>()
	{
		this->LightCBuffer = std::make_shared<GDirectionalLightCBuffer>(GRenderGroup::COMPOSITION_GROUP, 4);
	}

	GDirectionalLightRegistry::GDirectionalLightRegistry(const GDirectionalLightRegistry& other) : SInstantiableRegistry<GDirectionalLight>()
	{
		this->LightCBuffer = other.LightCBuffer;
	}

	GDirectionalLightRegistry::~GDirectionalLightRegistry()
	{

	}

	void GDirectionalLightRegistry::Register(std::shared_ptr<GDirectionalLight> instance)
	{
		SInstantiableRegistry::Register(instance);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(std::shared_ptr<GDirectionalLight> instance)
	{
		SInstantiableRegistry::Unregister(instance);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(std::string name)
	{
		SInstantiableRegistry::Unregister(name);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(SUUID uuid)
	{
		SInstantiableRegistry::Unregister(uuid);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::UpdateLightData()
	{
		GDirectionalLightCBufferData Data;
		Data.DirectionalLightCount = (int)this->RegisteredInstanceList.size();

		unsigned int index = 0;
		for (auto [uuid, light] : this->RegisteredInstanceList)
		{
			Data.DirectionalLightList[index] = light->LightData;
			index++;
		}

		this->LightCBuffer->UpdateData(Data);
	}
}
