#include <Core.h>
#include "StellarEngine.h"

namespace SE
{
	STELLAR_FINE_SINGLETON(StellarEngine, Instance);


	StellarEngine::StellarEngine()
	{

	}

	StellarEngine::~StellarEngine()
	{
		
	}

	void StellarEngine::InitializeEngine()
	{
		// Initialize all APIs for engine.
		SAPIConfigurator::InitializeAPI();

		// Initialize application of engine.
		FApplication::Instance->InitializeApplication();

		// Initialize the input module.
		FInput::InitializeInput(SWindowRegistry::GetMainInstance()->GetWindowHandle()->Instance);

		// Initialize the main graphics context for main window.
		this->MainGraphicsContext = GGraphicsContext::Create(SWindowRegistry::GetMainInstance()->GetWindowHandle(),
			SWindowRegistry::GetMainInstance()->GetWindowSize());
		SGraphicsContextRegistry::Register(SGraphicsContextRegistry::MainInstanceName, this->MainGraphicsContext);

		auto commandList = GCommandList::Create(this->MainGraphicsContext->GetDevice(), GCommandList::SE_COMMAND_LIST_DIRECT);
		SCommandListRegistry::Register("Test", commandList);
		SCommandListRegistry::SetCurrentInstance("Test");

		this->TestFramebuffer = GFramebuffer::Create(this->MainGraphicsContext->GetSwapChain());

		this->EngineEventProcesser = std::make_shared<FEventProcesser>();
		this->EngineEventProcesser->OnEvent<FWindowResizeEvent>([this](const FWindowResizeEvent& event)
			{
				if (event.WindowHandle == SWindowRegistry::GetMainInstance()->GetWindowHandle()->Instance &&
					event.ResizeWidth != 0 && event.ResizeHeight != 0)
				{
					this->TestFramebuffer->Resize({ event.ResizeWidth, event.ResizeHeight });
				}
			});

		// Post-Initialize those API which depend on other modules.
		SAPIConfigurator::PostInitializeAPI();
	}

	void StellarEngine::LaunchEngine()
	{
		SWindowRegistry::GetMainInstance()->Display();

		while (FApplication::Instance->IsApplicationRunning())
		{
			FApplication::Instance->UpdateApplication();
			FInput::UpdateInput();

			SCommandListRegistry::GetCurrentInstance()->Open();

			this->TestFramebuffer->Begin();
			this->TestFramebuffer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
			this->TestFramebuffer->Apply();
			this->TestFramebuffer->End();

			SCommandListRegistry::GetCurrentInstance()->Close();
			this->MainGraphicsContext->GetDevice()->ExecuteCommandLists({ SCommandListRegistry::GetCurrentInstance()->GetInstance().Get() });
			this->MainGraphicsContext->Flush();
			this->MainGraphicsContext->Present(true);
		}
	}

	void StellarEngine::ShutdownEngine()
	{
		SAPIConfigurator::ShutdownAPI();
	}
}
