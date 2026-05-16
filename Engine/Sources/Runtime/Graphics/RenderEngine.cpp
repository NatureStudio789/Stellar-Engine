#include <Core.h>
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"
#include "PipelineState/PipelineState.h"
#include "Material/StandardMaterial.h"
#include "RenderEngine.h"

namespace SE
{
	STELLAR_REGISTER_ENGINE(RenderEngine);
	

	RenderEngine::RenderEngine()
	{
		this->EngineName = "#1_Render_Engine";
	}

	void RenderEngine::Initialize()
	{
		GRenderConfiguration::InitializeConfiguration();

		this->MainGraphicsContext = GGraphicsContext::Create(
			SWindowRegistry::GetMainInstance()->GetWindowHandle(),
			SWindowRegistry::GetMainInstance()->GetWindowSize());
		this->MainGraphicsContext->SetName(SGraphicsContextRegistry::MainInstanceName);
		SGraphicsContextRegistry::Register(this->MainGraphicsContext);

		SPipelineStateRegistry::InitializeRegistry();

		auto& DeferredRenderer = std::make_shared<GDeferredRenderer>("MainDeferredRenderer");
		DeferredRenderer->Compile();
		SRendererRegistry::Register(DeferredRenderer);

		std::shared_ptr<GStandardMaterial> testmat = std::make_shared<GStandardMaterial>("testmat");
		SMaterialRegistry::Register(testmat);

		{
			GMeshItem::Data data;
			data.Vertices =
			{
				GMeshItem::Vertex{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
				GMeshItem::Vertex{{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
				GMeshItem::Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}
			};
			data.Indices = { 0, 1, 2 };
			data.MaterialId = testmat->GetUUID();
			this->test = std::make_shared<GMeshItem>("test", data);

			this->test->SetTransform({ glm::vec3{0.2f, 0.0f, 0.0f}, glm::quat{}, glm::vec3{1.0f, 1.0f, 1.0f} });
			this->test->LinkTechnique("MainDeferredRenderer");
		}

		{
			GMeshItem::Data data;
			data.Vertices =
			{
				GMeshItem::Vertex{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
				GMeshItem::Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
				GMeshItem::Vertex{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
			};
			data.Indices = { 0, 1, 2 };
			data.MaterialId = testmat->GetUUID();
			this->test1 = std::make_shared<GMeshItem>("test1", data);

			this->test1->SetTransform({ glm::vec3{-0.2f, 0.0f, 0.0f}, glm::quat{}, glm::vec3{1.0f, 1.0f, 1.0f} });
			this->test1->LinkTechnique("MainDeferredRenderer");
		}
	}

	void RenderEngine::Execute()
	{
		this->test->Submit("main");
		this->test1->Submit("main");

		for (auto& [uuid, renderer] : SRendererRegistry::GetInstanceList())
		{
			renderer->Execute();
		}

		std::vector<ID3D12GraphicsCommandList*> ExecutingCommandList;
		for (auto& [uuid, commandList] : SCommandListRegistry::GetInstanceList())
		{
			ExecutingCommandList.push_back(commandList->GetInstance().Get());
		}
		SGraphicsContextRegistry::GetMainInstance()->ExecuteCommandLists(ExecutingCommandList);

		SGraphicsContextRegistry::GetMainInstance()->Present(true);
	}

	void RenderEngine::Release()
	{

	}
}