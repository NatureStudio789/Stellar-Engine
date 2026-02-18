#include <Core.h>
#include "../Context/GraphicsContext.h"
#include "ContextDependent.h"

namespace SE
{
	void GContextDependent::SetDependentContext(const std::string& name)
	{
		this->DependentContextName = name;
	}

	std::shared_ptr<GGraphicsContext> GContextDependent::GetContext()
	{
		return SGraphicsContextRegistry::GetInstance(this->DependentContextName);
	}
}
