#include <Core.h>
#include "../../../RenderConfiguration/RenderConfiguration.h"
#include "MeshItem.h"

namespace SE
{
	GMeshItem::GMeshItem()
	{
		this->ItemData = {};
	}

	GMeshItem::GMeshItem(const std::string& name, const Data& data)
	{
		this->Initialize(name, data);
	}

	GMeshItem::~GMeshItem()
	{

	}

	void GMeshItem::Initialize(const std::string& name, const Data& data)
	{
		this->ItemData = data;

		GRenderable::Initialize(name, 
			GVertexBuffer::Create((void*)this->ItemData.Vertices.data(), (unsigned int)this->ItemData.Vertices.size(), (unsigned int)sizeof(Vertex)), 
			GIndexBuffer::Create(this->ItemData.Indices.data(), (unsigned int)this->ItemData.Vertices.size()), GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

		auto& LightingTechnique = std::make_shared<GRenderTechnique>("LightingTechnique", "main");

		// Depend on the render configuration, apply lighting rendering stage to renderer.
		if (GRenderConfiguration::GetRenderingParadigm() == GRenderConfiguration::SE_FORWARD_RENDERING)
		{
			auto& LightingStage = std::make_shared<GRenderStage>("Lighting");

			// Add applicable to stages;
			LightingStage;

			LightingTechnique->AddRenderStage(LightingStage);
		}
		else if (GRenderConfiguration::GetRenderingParadigm() == GRenderConfiguration::SE_DEFERRED_RENDERING)
		{
			auto& AlbedoStage = std::make_shared<GRenderStage>("AlbedoBuffer");

			// Add applicable to stages;
			AlbedoStage->AddApplicable(GTransformCBuffer::Create(GRenderGroup::ALBEDO_GROUP, 0));

			LightingTechnique->AddRenderStage(AlbedoStage);

			//auto& MetallicStage = std::make_shared<GRenderStage>("MetallicBuffer");

			//// Add applicable to stages;

			//LightingTechnique->AddRenderStage(MetallicStage);

			//auto& RoughnessStage = std::make_shared<GRenderStage>("RoughnessBuffer");

			//// Add applicable to stages;

			//LightingTechnique->AddRenderStage(RoughnessStage);

			//auto& NormalStage = std::make_shared<GRenderStage>("NormalBuffer");

			//// Add applicable to stages;

			//LightingTechnique->AddRenderStage(NormalStage);
		}

		this->AddRenderTechnique(LightingTechnique);
	}

	const GMeshItem::Data& GMeshItem::GetData() const noexcept
	{
		return this->ItemData;
	}
}
