#include <Core.h>
#include "UUID.h"

namespace SE
{
	SUUID::SUUID()
	{
		this->UUID = "";
		this->IsAvailable = false;
	}

	SUUID::SUUID(const SUUID& other)
	{
		this->UUID = other.UUID;
		this->IsAvailable = other.IsAvailable;
	}

	SUUID::~SUUID()
	{
		this->Destroy();
	}

	void SUUID::Activate()
	{
		this->UUID = SUUIDGenerator::Generate();
		this->IsAvailable = true;
	}

	void SUUID::Destroy()
	{
		this->UUID.clear();
		this->IsAvailable = false;
	}

	const std::string& SUUID::operator()() const noexcept
	{
		return this->UUID;
	}

	bool SUUID::operator<(const SUUID& other) const
	{
		return this->UUID[0] < other.UUID[0];
	}

	bool SUUID::GetAvailable() const noexcept
	{
		return this->IsAvailable;
	}
}
