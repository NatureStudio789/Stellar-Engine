#include <Core.h>
#include "Outflow.h"

namespace SE
{
	GOutflow::GOutflow(const std::string& name, GResourcePackage& package) : ResourcePackage(package)
	{
		this->OutflowName = name;
	}

	GOutflow::GOutflow(const GOutflow& other) : ResourcePackage(other.ResourcePackage)
	{
		this->OutflowName = other.OutflowName;
	}

	GOutflow::~GOutflow()
	{
		
	}

	const std::string& GOutflow::GetName() const noexcept
	{
		return this->OutflowName;
	}

	GResourcePackage& GOutflow::GetResourcePackage()
	{
		return this->ResourcePackage;
	}
}
