#include <Core.h>
#include "RootParameter.h"

namespace SE
{
	GRootParameter::GRootParameter()
	{
		this->ShaderRegisterIndex = 0;
		this->DescriptorCount = 1;
	}

	GRootParameter::GRootParameter(Type type, unsigned int index, unsigned int descriptorCount)
	{
		this->ParameterType = type;
		this->ShaderRegisterIndex = index;
		this->DescriptorCount = descriptorCount;
	}

	GRootParameter::GRootParameter(const GRootParameter& other)
	{
		this->ParameterType = other.ParameterType;
		this->ShaderRegisterIndex = other.ShaderRegisterIndex;
		this->DescriptorCount = other.DescriptorCount;
	}

	bool GRootParameter::operator==(const GRootParameter& other) const noexcept
	{
		return this->ParameterType == other.ParameterType && 
			this->ShaderRegisterIndex == other.ShaderRegisterIndex &&
			this->DescriptorCount == other.DescriptorCount;
	}
}
