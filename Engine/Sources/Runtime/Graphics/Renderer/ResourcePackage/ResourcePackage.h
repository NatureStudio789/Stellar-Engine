#ifndef _SE_RESOURCEPACKAGE_H_
#define _SE_RESOURCEPACKAGE_H_
#include "../../../Core/Addressable/Addressable.h"

namespace SE
{
	class GResourcePackage
	{
	public:
		enum Type
		{
			SE_RESOURCE_FRAMEBUFFER,
			SE_RESOURCE_TEXTURE,
		};

	public:
		GResourcePackage();
		GResourcePackage(const SAddressable::Identifier& identifier, Type type);
		GResourcePackage(const GResourcePackage& other);
		~GResourcePackage();

		void Initialize(const SAddressable::Identifier& identifier, Type type);

		const SAddressable::Identifier& GetResourceIdentifier() const noexcept;
		const Type& GetResourceType() const noexcept;

	private:
		SAddressable::Identifier ResourceIdentifier;
		Type ResourceType;
	};
}

#endif