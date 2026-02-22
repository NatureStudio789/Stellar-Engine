#include <Core.h>
#include "Addressable.h"

namespace SE
{
	SAddressable::SAddressable()
	{
		this->IdentifierHandle = {};
	}

	SAddressable::SAddressable(const SAddressable& other)
	{
		this->IdentifierHandle = other.IdentifierHandle;
	}

	void SAddressable::SetName(const std::string& name)
	{
		this->IdentifierHandle.Name = name;
	}

	const SAddressable::Identifier& SAddressable::GetIdentifier() const noexcept
	{
		return this->IdentifierHandle;
	}

	const SUUID& SAddressable::GetUUID() const noexcept
	{
		return this->IdentifierHandle.UUID;
	}

	const std::string& SAddressable::GetName() const noexcept
	{
		return this->IdentifierHandle.Name;
	}

	void SAddressable::Activate()
	{
		this->IdentifierHandle.UUID.Activate();
	}
}
