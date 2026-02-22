#ifndef _SE_ADDRESSABLE_H_
#define _SE_ADDRESSABLE_H_
#include "../UUID/UUID.h"

namespace SE
{
	class SAddressable
	{
	public:
		struct Identifier
		{
			Identifier()
			{
				this->Name = "";
				this->UUID = {};
			}

			Identifier(const Identifier& other)
			{
				this->Name = other.Name;
				this->UUID = other.UUID;
			}

			const std::string GetName() const noexcept
			{
				return this->Name;
			}

			const SUUID& GetUUID() const noexcept
			{
				return this->UUID;
			}

		private:
			std::string Name;
			SUUID UUID;

			friend class SAddressable;
		};

	public:
		SAddressable();
		SAddressable(const SAddressable& other);

		void SetName(const std::string& name);

		const Identifier& GetIdentifier() const noexcept;
		const SUUID& GetUUID() const noexcept;
		const std::string& GetName() const noexcept;

	protected:
		void Activate();

		Identifier IdentifierHandle;
	};
}

#endif