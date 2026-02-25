#include <Core.h>
#include "ResourcePackage.h"

namespace SE
{
	GResourcePackage::GResourcePackage()
	{
		this->ResourceIdentifier = {};
	}

	GResourcePackage::GResourcePackage(const SAddressable::Identifier& identifier, Type type)
	{
		this->Initialize(identifier, type);
	}

	GResourcePackage::GResourcePackage(const GResourcePackage& other)
	{
		this->ResourceIdentifier = other.ResourceIdentifier;
		this->ResourceType = other.ResourceType;
	}

	GResourcePackage::~GResourcePackage()
	{
		
	}

	void GResourcePackage::Initialize(const SAddressable::Identifier& identifier, Type type)
	{
		this->ResourceIdentifier = identifier;
		this->ResourceType = type;
	}

	const SAddressable::Identifier& GResourcePackage::GetResourceIdentifier() const noexcept
	{
		return this->ResourceIdentifier;
	}

	const GResourcePackage::Type& GResourcePackage::GetResourceType() const noexcept
	{
		return this->ResourceType;
	}
}
