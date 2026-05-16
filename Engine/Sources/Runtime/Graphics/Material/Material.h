#ifndef _SE_MATERIAL_H_
#define _SE_MATERIAL_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Registry/Registry.h"

#include "../Applicable/Applicable.h"
#include "../Texture/Texture.h"

namespace SE
{
	class GMaterial : public SAddressable, public GApplicable
	{
	public:
		GMaterial();
		GMaterial(const std::string& name);
		GMaterial(const GMaterial& other);
		~GMaterial() override;

		void AddTexture(const std::string& name, std::shared_ptr<GTexture> texture);

		virtual void Apply() override;

		std::shared_ptr<GTexture> GetTexture(const std::string& name);

	protected:
		std::map<std::string, std::shared_ptr<GTexture>> TextureList;
	};

	STELLAR_MAKE_BLANK_REGISTRY(GMaterial, MaterialRegistry);
}

#endif