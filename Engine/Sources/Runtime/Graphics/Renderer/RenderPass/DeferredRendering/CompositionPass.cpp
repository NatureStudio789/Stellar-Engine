#include <Core.h>
#include "../../../Framebuffer/Framebuffer.h"
#include "../../../Buffer/ConstantBuffer.h"
#include "../../../PipelineState/PipelineState.h"
#include "../../../ShaderResourceView/SRVSet.h"

#include "../../Renderable/Renderable.h"
#include "CompositionPass.h"

namespace SE
{
	GCompositionPass::GCompositionPass() : GRenderQueuePass()
	{

	}

	GCompositionPass::GCompositionPass(const std::string& name) : GRenderQueuePass()
	{
		this->Initialize(name);
	}

	GCompositionPass::GCompositionPass(const GCompositionPass & other) : GRenderQueuePass(other)
	{

	}

	GCompositionPass::~GCompositionPass()
	{

	}

	void GCompositionPass::Initialize(const std::string& name)
	{
		this->RenderPassName = name;
		this->EnableUniversalCameraForRendering = false;

		auto CompositionFramebufferInflow = GInflow::Create("CompositionFramebuffer", this->FramebufferPackage);
		this->AddInflow(CompositionFramebufferInflow);

		auto GBufferInflow = GInflow::Create("GBuffer", this->GBufferPackage);
		this->AddInflow(GBufferInflow);

		auto DirectionalLightDepthMapInflow = GInflow::Create("DirectionalDepthMapList", this->DirectionalLightDepthMapSetPackage);
		this->AddInflow(DirectionalLightDepthMapInflow);

		this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP));

		struct PresenterVertex
		{
			PresenterVertex() = default;
			PresenterVertex(const glm::vec2& position, const glm::vec2& textureCoord) 
			{
				this->Position = position;
				this->TextureCoord = textureCoord;
			}
			PresenterVertex(const PresenterVertex&) = default;

			glm::vec2 Position;
			glm::vec2 TextureCoord;
		};

		std::vector<PresenterVertex> Vertices =
		{
			{glm::vec2{-1.0f,  1.0f}, glm::vec2{0.0f, 0.0f}},
			{glm::vec2{ 1.0f,  1.0f}, glm::vec2{1.0f, 0.0f}},
			{glm::vec2{-1.0f, -1.0f}, glm::vec2{0.0f, 1.0f}},
			{glm::vec2{ 1.0f, -1.0f}, glm::vec2{1.0f, 1.0f}},
		};

		std::vector<unsigned int> Indices =
		{
			0, 1, 2,
			1, 3, 2
		};

		this->FramebufferPresenter = std::make_shared<GRenderable>("FramebufferPresenter",
			std::make_shared<GVertexBuffer>((void*)Vertices.data(), (unsigned int)Vertices.size(), (unsigned int)sizeof(PresenterVertex)), 
			std::make_shared<GIndexBuffer>(Indices.data(), (unsigned int)Indices.size()), 
			GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

		this->AttributionCBuffer = std::make_shared<GConstantBuffer<AttriCBufferData>>(GRenderGroup::COMPOSITION_GROUP);

		auto CompositionTechnique = std::make_shared<GRenderTechnique>("CompositionTechnique", "main");

		auto CompositionStage = std::make_shared<GRenderStage>(name);
		CompositionStage->AddApplicable(this->AttributionCBuffer);
		CompositionTechnique->AddRenderStage(CompositionStage);

		this->FramebufferPresenter->AddRenderTechnique(CompositionTechnique);
	}

	void GCompositionPass::Execute()
	{
		if (!this->IsSetup)
		{
			this->ExecuteSetup();
			this->IsSetup = true;
		}

		this->FramebufferPresenter->Submit(this->FramebufferPresenter, "main");
		
		AttriCBufferData data;
		data.ViewPosition = SCameraRegistry::GetCurrentInstance()->Position;
		this->AttributionCBuffer->UpdateData(data);

		GRenderQueuePass::Execute();
	}

	void GCompositionPass::ExecuteSetup()
	{
		auto GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->GBufferPackage.GetResourceIdentifier().GetUUID());

		auto AlbedoSRV = GBufferFramebuffer->GetRTShaderResourceView(0);
		AlbedoSRV->SetRootParameterIndex(0, GRenderGroup::COMPOSITION_GROUP);

		auto MetallicSRV = GBufferFramebuffer->GetRTShaderResourceView(1);
		MetallicSRV->SetRootParameterIndex(1, GRenderGroup::COMPOSITION_GROUP);

		auto RoughnessSRV = GBufferFramebuffer->GetRTShaderResourceView(2);
		RoughnessSRV->SetRootParameterIndex(2, GRenderGroup::COMPOSITION_GROUP);

		auto NormalSRV = GBufferFramebuffer->GetRTShaderResourceView(3);
		NormalSRV->SetRootParameterIndex(3, GRenderGroup::COMPOSITION_GROUP);

		auto PositionSRV = GBufferFramebuffer->GetRTShaderResourceView(4);
		PositionSRV->SetRootParameterIndex(4, GRenderGroup::COMPOSITION_GROUP);

		this->AddApplicable(AlbedoSRV);
		this->AddApplicable(MetallicSRV);
		this->AddApplicable(RoughnessSRV);
		this->AddApplicable(NormalSRV);
		this->AddApplicable(PositionSRV);

		auto DirectionalDepthMapSet = SSRVSetRegistry::GetInstance(this->DirectionalLightDepthMapSetPackage.GetResourceIdentifier().GetUUID());
		this->AddApplicable(DirectionalDepthMapSet);

		this->FramebufferPresenter->LinkTechnique("MainDeferredRenderer");
	}
}
