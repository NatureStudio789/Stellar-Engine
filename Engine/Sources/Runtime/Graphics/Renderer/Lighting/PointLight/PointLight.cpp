#include <Core.h>
#include "PointLight.h"

namespace SE
{
    GPointLight::GPointLight() : SAddressable()
    {
        this->LightData = {};

        this->Activate();
    }

    GPointLight::GPointLight(const std::string& name, const Data& data) : SAddressable()
    {
        this->SetName(name);

        this->LightData = data;

        this->Activate();
    }

    GPointLight::GPointLight(const GPointLight & other) : SAddressable(other)
    {
        this->LightData = other.LightData;
    }

    GPointLight::~GPointLight()
    {
          
    }
}
