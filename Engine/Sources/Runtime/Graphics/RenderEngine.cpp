#include <Core.h>
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"
#include "PipelineState/PipelineState.h"
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
		this->MainGraphicsContext = GGraphicsContext::Create(
			SWindowRegistry::GetMainInstance()->GetWindowHandle(),
			SWindowRegistry::GetMainInstance()->GetWindowSize());
		this->MainGraphicsContext->SetName(SGraphicsContextRegistry::MainInstanceName);
		SGraphicsContextRegistry::Register(this->MainGraphicsContext);

		SPipelineStateRegistry::InitializeRegistry();

		auto& DeferredRenderer = std::make_shared<GDeferredRenderer>("MainDeferredRenderer");
		DeferredRenderer->Compile();
		SRendererRegistry::Register(DeferredRenderer);
	}

	void RenderEngine::Execute()
	{
		for (auto& [uuid, renderer] : SRendererRegistry::GetInstanceList())
		{
			renderer->Execute();
		}

		SGraphicsContextRegistry::GetMainInstance()->Present(true);
	}

	void RenderEngine::Release()
	{

	}
}