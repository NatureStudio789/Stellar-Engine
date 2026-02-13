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
	}

	void StellarEngine::LaunchEngine()
	{
		SWindowRegistry::GetMainInstance()->Display();

		while (FApplication::Instance->IsApplicationRunning())
		{
			FApplication::Instance->UpdateApplication();
			FInput::UpdateInput();
		}
	}

	void StellarEngine::ShutdownEngine()
	{
		SAPIConfigurator::ShutdownAPI();
	}
}
