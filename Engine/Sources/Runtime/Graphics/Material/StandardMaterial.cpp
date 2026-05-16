#include <Core.h>
#include "../RenderGroup/RenderGroup.h"
#include "StandardMaterial.h"

namespace SE
{
	const std::string& GStandardMaterial::AlbedoSlotName = "StdMat_Albedo";
	const std::string& GStandardMaterial::MetallicSlotName = "StdMat_Metallic";
	const std::string& GStandardMaterial::RoughnessSlotName = "StdMat_Roughness";
	const std::string& GStandardMaterial::NormalSlotName = "StdMat_Normal";
	
	GStandardMaterial::GStandardMaterial() : GMaterial()
	{

	}

	GStandardMaterial::GStandardMaterial(const std::string& name) : GMaterial(name)
	{
		this->MaterialCBuffer = std::make_shared<GConstantBuffer<Data>>(GRenderGroup::ALBEDO_GROUP, 1);

		this->MaterialData.HasAlbedoTexture = false;
		this->MaterialData.Albedo = { 0.3f, 0.3f, 0.3f };
		this->MaterialData.HasMetallicTexture = false;
		this->MaterialData.Metallic = 0.0f;
		this->MaterialData.HasRoughnessTexture = false;
		this->MaterialData.Roughness = 0.5f;
		this->MaterialData.HasNormalTexture = false;
	}

	GStandardMaterial::GStandardMaterial(const GStandardMaterial & other) : GMaterial(other)
	{
		this->MaterialData = other.MaterialData;
		this->MaterialCBuffer = other.MaterialCBuffer;
	}

	GStandardMaterial::~GStandardMaterial()
	{
		this->MaterialCBuffer.reset();
	}

	void GStandardMaterial::SetAlbedo(std::shared_ptr<GTexture> texture)
	{
		this->TextureList[AlbedoSlotName] = texture;
		this->MaterialData.HasAlbedoTexture = true;
	}

	void GStandardMaterial::SetAlbedo(const glm::vec3 & albedo)
	{
		this->MaterialData.Albedo = albedo;
	}

	void GStandardMaterial::RemoveAlbedoTexture() noexcept
	{
		if (this->TextureList.count(AlbedoSlotName) > 0)
		{
			this->TextureList.erase(AlbedoSlotName);
		}

		this->MaterialData.HasAlbedoTexture = false;
	}

	void GStandardMaterial::SetMetallic(std::shared_ptr<GTexture> texture)
	{
		this->TextureList[MetallicSlotName] = texture;
		this->MaterialData.HasMetallicTexture = true;
	}

	void GStandardMaterial::SetMetallic(float metallic)
	{
		this->MaterialData.Metallic = metallic;
	}

	void GStandardMaterial::RemoveMetallicTexture() noexcept
	{
		if (this->TextureList.count(MetallicSlotName) > 0)
		{
			this->TextureList.erase(MetallicSlotName);
		}

		this->MaterialData.HasMetallicTexture = false;
	}

	void GStandardMaterial::SetRoughness(std::shared_ptr<GTexture> texture)
	{
		this->TextureList[RoughnessSlotName] = texture;
		this->MaterialData.HasRoughnessTexture = true;
	}

	void GStandardMaterial::SetRoughness(float roughness)
	{
		this->MaterialData.Roughness = roughness;
	}

	void GStandardMaterial::RemoveRoughnessTexture() noexcept
	{
		if (this->TextureList.count(RoughnessSlotName) > 0)
		{
			this->TextureList.erase(RoughnessSlotName);
		}

		this->MaterialData.HasRoughnessTexture = true;
	}

	void GStandardMaterial::SetNormalTexture(std::shared_ptr<GTexture> texture)
	{
		this->TextureList[NormalSlotName] = texture;
		this->MaterialData.HasNormalTexture = true;
	}

	void GStandardMaterial::RemoveNormalTexture() noexcept
	{
		if (this->TextureList.count(NormalSlotName) > 0)
		{
			this->TextureList.erase(NormalSlotName);
		}

		this->MaterialData.HasNormalTexture = false;
	}

	void GStandardMaterial::Apply()
	{
		GMaterial::Apply();

		this->MaterialCBuffer->UpdateData(this->MaterialData);
		this->MaterialCBuffer->Apply();
	}
}
