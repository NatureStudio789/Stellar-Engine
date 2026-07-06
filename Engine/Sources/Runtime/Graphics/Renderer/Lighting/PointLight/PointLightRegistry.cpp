#include <Core.h>
#include "../../../RenderGroup/RenderGroup.h"
#include "PointLightRegistry.h"

namespace SE
{
	GPointLightRegistry::GPointLightRegistry() : SInstantiableRegistry()
	{
		this->LightCBuffer = std::make_shared<GPointLightCBuffer>(GRenderGroup::COMPOSITION_GROUP, 3);
	}

	GPointLightRegistry::GPointLightRegistry(const GPointLightRegistry& other) : SInstantiableRegistry(other)
	{
		this->LightCBuffer = other.LightCBuffer;
	}

	GPointLightRegistry::~GPointLightRegistry()
	{

	}

	void GPointLightRegistry::Register(std::shared_ptr<GPointLight> instance)
	{
		SInstantiableRegistry::Register(instance);

		this->UpdateLightData();
	}

	void GPointLightRegistry::Unregister(std::shared_ptr<GPointLight> instance)
	{
		SInstantiableRegistry::Unregister(instance);

		this->UpdateLightData();
	}

	void GPointLightRegistry::Unregister(std::string name)
	{
		SInstantiableRegistry::Unregister(name);

		this->UpdateLightData();
	}

	void GPointLightRegistry::Unregister(SUUID uuid)
	{
		SInstantiableRegistry::Unregister(uuid);

		this->UpdateLightData();
	}

	void GPointLightRegistry::UpdateLightData()
	{
		GPointLightCBufferData Data;
		Data.PointLightCount = (int)this->RegisteredInstanceList.size();

		unsigned int index = 0;
		for (auto& [uuid, light] : this->RegisteredInstanceList)
		{
			Data.PointLightList[index] = light->LightData;
			index++;
		}

		this->LightCBuffer->UpdateData(Data);
	}
}
