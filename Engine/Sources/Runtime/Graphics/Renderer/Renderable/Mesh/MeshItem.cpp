#include <Core.h>
#include "../../../RenderConfiguration/RenderConfiguration.h"
#include "../../../Material/Material.h"
#include "MeshItem.h"

namespace SE
{
	GMeshItem::GMeshItem()
	{
		this->ItemData = {};
	}

	GMeshItem::GMeshItem(const Data& data)
	{
		this->Initialize(data);
	}

	GMeshItem::~GMeshItem()
	{

	}

	void GMeshItem::Initialize(const Data& data)
	{
		this->ItemData = data;

		GRenderable::Initialize(this->ItemData.Name, 
			GVertexBuffer::Create((void*)this->ItemData.Vertices.data(), (unsigned int)this->ItemData.Vertices.size(), (unsigned int)sizeof(Vertex)), 
			GIndexBuffer::Create(this->ItemData.Indices.data(), (unsigned int)this->ItemData.Indices.size()), GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

		auto LightingTechnique = std::make_shared<GRenderTechnique>("LightingTechnique", "main");

		// Depend on the render configuration, apply lighting rendering stage to renderer.
		if (GRenderConfiguration::GetRenderingParadigm() == GRenderConfiguration::SE_FORWARD_RENDERING)
		{
			auto LightingStage = std::make_shared<GRenderStage>("Lighting");

			// Add applicable to stages;
			LightingStage;

			LightingTechnique->AddRenderStage(LightingStage);
		}
		else if (GRenderConfiguration::GetRenderingParadigm() == GRenderConfiguration::SE_DEFERRED_RENDERING)
		{
			auto AlbedoStage = std::make_shared<GRenderStage>("AlbedoBuffer");

			// Add applicable to stages;
			AlbedoStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::ALBEDO_GROUP, 0));
			AlbedoStage->AddApplicable(SMaterialRegistry::GetInstance(this->ItemData.MaterialId));

			LightingTechnique->AddRenderStage(AlbedoStage);

			auto MetallicStage = std::make_shared<GRenderStage>("MetallicBuffer");

			// Add applicable to stages;
			MetallicStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::METALLIC_GROUP, 0));
			MetallicStage->AddApplicable(SMaterialRegistry::GetInstance(this->ItemData.MaterialId));

			LightingTechnique->AddRenderStage(MetallicStage);

			auto RoughnessStage = std::make_shared<GRenderStage>("RoughnessBuffer");

			// Add applicable to stages;
			RoughnessStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::ROUGHNESS_GROUP, 0));
			RoughnessStage->AddApplicable(SMaterialRegistry::GetInstance(this->ItemData.MaterialId));

			LightingTechnique->AddRenderStage(RoughnessStage);

			auto NormalStage = std::make_shared<GRenderStage>("NormalBuffer");

			// Add applicable to stages;
			NormalStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::NORMAL_GROUP, 0));
			NormalStage->AddApplicable(SMaterialRegistry::GetInstance(this->ItemData.MaterialId));

			LightingTechnique->AddRenderStage(NormalStage);

			auto PositionStage = std::make_shared<GRenderStage>("PositionBuffer");

			// Add applicable to stages;
			PositionStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::POSITION_GROUP, 0));

			LightingTechnique->AddRenderStage(PositionStage);
		}

		this->AddRenderTechnique(LightingTechnique);
	}

	const GMeshItem::Data& GMeshItem::GetData() const noexcept
	{
		return this->ItemData;
	}
}
