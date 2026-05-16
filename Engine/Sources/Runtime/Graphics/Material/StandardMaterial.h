#ifndef _SE_STANDARDMATERTIAL_H_
#define _SE_STANDARDMATERTIAL_H_
#include "Material.h"

#include "../Buffer/ConstantBuffer.h"

namespace SE
{
	class GStandardMaterial : public GMaterial
	{
	public:
		// This Data class is for Constant Buffer uploading.
		struct Data
		{
			glm::vec3 Albedo;
			float HasAlbedoTexture;
			float Metallic;
			float HasMetallicTexture;
			float Roughness;
			float HasRoughnessTexture;
			float HasNormalTexture;
		};

	public:
		GStandardMaterial();
		GStandardMaterial(const std::string& name);
		GStandardMaterial(const GStandardMaterial& other);
		~GStandardMaterial() override;

		void SetAlbedo(std::shared_ptr<GTexture> texture);
		void SetAlbedo(const glm::vec3& albedo);
		void RemoveAlbedoTexture() noexcept;

		void SetMetallic(std::shared_ptr<GTexture> texture);
		void SetMetallic(float metallic);
		void RemoveMetallicTexture() noexcept;

		void SetRoughness(std::shared_ptr<GTexture> texture);
		void SetRoughness(float roughness);
		void RemoveRoughnessTexture() noexcept;

		void SetNormalTexture(std::shared_ptr<GTexture> texture);
		void RemoveNormalTexture() noexcept;

		void Apply() override;

		static const std::string& AlbedoSlotName;
		static const std::string& MetallicSlotName;
		static const std::string& RoughnessSlotName;
		static const std::string& NormalSlotName;

	private:
		Data MaterialData;
		std::shared_ptr<GConstantBuffer<Data>> MaterialCBuffer;

	};
}

#endif