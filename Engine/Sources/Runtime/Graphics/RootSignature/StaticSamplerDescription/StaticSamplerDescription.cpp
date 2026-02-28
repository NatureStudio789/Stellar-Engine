#include <Core.h>
#include "StaticSamplerDescription.h"

namespace SE
{
	GStaticSamplerDescription::GStaticSamplerDescription(unsigned int index, D3D12_FILTER filter,
		D3D12_TEXTURE_ADDRESS_MODE addressU,
		D3D12_TEXTURE_ADDRESS_MODE addressV,
		D3D12_TEXTURE_ADDRESS_MODE addressW)
	{
		this->ShaderRegister = index;
		this->Filter = filter;
		this->AddressU = addressU;
		this->AddressV = addressV;
		this->AddressW = addressW;
	}

	GStaticSamplerDescription::GStaticSamplerDescription(const GStaticSamplerDescription& other)
	{
		this->ShaderRegister = other.ShaderRegister;
		this->Filter = other.Filter;
		this->AddressU = other.AddressU;
		this->AddressV = other.AddressV;
		this->AddressW = other.AddressW;
	}

	bool GStaticSamplerDescription::operator==(const GStaticSamplerDescription& other) const noexcept
	{
		return this->ShaderRegister == other.ShaderRegister && this->Filter == other.Filter && this->AddressU == other.AddressU
			&& this->AddressV == other.AddressV && this->AddressW == this->AddressW;
	}
}
