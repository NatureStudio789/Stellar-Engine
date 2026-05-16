#include <Core.h>
#include "Material.h"

namespace SE
{
	GMaterial::GMaterial() : SAddressable(), GApplicable()
	{

	}

	GMaterial::GMaterial(const std::string& name) : GApplicable()
	{
		this->SetName(name);

		this->Activate();
	}

	GMaterial::GMaterial(const GMaterial& other) : SAddressable(other), GApplicable(other)
	{
		this->TextureList = other.TextureList;
	}

	GMaterial::~GMaterial()
	{
		this->TextureList.clear();
	}

	void GMaterial::AddTexture(const std::string& name, std::shared_ptr<GTexture> texture)
	{
		if (this->TextureList.count(name) > 0)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("Texture named '{}' has already been in the material '{}'", name, this->GetName()));
		}

		this->TextureList[name] = texture;
	}

	void GMaterial::Apply()
	{
		for (auto& [name, texture] : this->TextureList)
		{
			texture->Apply();
		}
	}

	std::shared_ptr<GTexture> GMaterial::GetTexture(const std::string& name)
	{
		if (this->TextureList.count(name) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("No texture named '{}' found in the material '{}'", name, this->GetName()));
		}

		return this->TextureList[name];
	}
}
