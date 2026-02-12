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
		FInput::InitializeInput(SWindowRegistry::GetMainFWindow()->GetWindowHandle()->Instance);
	}

	void StellarEngine::LaunchEngine()
	{
		SWindowRegistry::GetMainFWindow()->Display();

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
