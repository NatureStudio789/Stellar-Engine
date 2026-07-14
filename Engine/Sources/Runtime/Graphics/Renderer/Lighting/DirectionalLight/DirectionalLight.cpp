#include <Core.h>
#include "DirectionalLight.h"

namespace SE
{
	GDirectionalLight::GDirectionalLight() : SAddressable()
	{
		this->LightData = {};

		this->Activate();
	}

	GDirectionalLight::GDirectionalLight(const std::string& name, const Data& data) : SAddressable()
	{
		this->LightData = data;
		this->SetName(name);

		this->Activate();
	}

	GDirectionalLight::GDirectionalLight(const GDirectionalLight & other) : SAddressable(other)
	{
		this->LightData = other.LightData;
	}

	GDirectionalLight::~GDirectionalLight()
	{
		
	}
}
