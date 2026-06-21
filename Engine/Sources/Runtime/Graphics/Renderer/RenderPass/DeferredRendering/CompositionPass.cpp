#include <Core.h>
#include "../../../Framebuffer/Framebuffer.h"
#include "../../../PipelineState/PipelineState.h"

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
		this->EnableCamera = false;

		auto& CompositionFramebufferInflow = GInflow::Create("CompositionFramebuffer", this->FramebufferPackage);
		this->AddInflow(CompositionFramebufferInflow);

		auto& GBufferInflow = GInflow::Create("GBuffer", this->GBufferPackage);
		this->AddInflow(GBufferInflow);

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

		auto& CompositionTechnique = std::make_shared<GRenderTechnique>("CompositionTechnique", "main");

		auto& CompositionStage = std::make_shared<GRenderStage>(name);
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

		GRenderQueuePass::Execute();
	}

	void GCompositionPass::ExecuteSetup()
	{
		auto& GBufferFramebuffer = SFramebufferRegistry::GetInstance(this->GBufferPackage.GetResourceIdentifier().GetUUID());

		auto& AlbedoSRV = GBufferFramebuffer->GetRTShaderResourceView(0);
		AlbedoSRV->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->GetRootSignature()->
			GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 0)));

		auto& MetallicSRV = GBufferFramebuffer->GetRTShaderResourceView(1);
		MetallicSRV->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->GetRootSignature()->
			GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 1)));

		auto& RoughnessSRV = GBufferFramebuffer->GetRTShaderResourceView(2);
		RoughnessSRV->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->GetRootSignature()->
			GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 2)));

		auto& NormalSRV = GBufferFramebuffer->GetRTShaderResourceView(3);
		NormalSRV->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->GetRootSignature()->
			GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 3)));

		auto& PositionSRV = GBufferFramebuffer->GetRTShaderResourceView(4);
		PositionSRV->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->GetRootSignature()->
			GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 4)));

		this->AddApplicable(AlbedoSRV);
		this->AddApplicable(MetallicSRV);
		this->AddApplicable(RoughnessSRV);
		this->AddApplicable(NormalSRV);
		this->AddApplicable(PositionSRV);

		this->FramebufferPresenter->LinkTechnique("MainDeferredRenderer");
	}
}
