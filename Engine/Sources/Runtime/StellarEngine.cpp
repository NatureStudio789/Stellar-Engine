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

		// Initialize all engine modules.
		SEngineSystem::InitializeAll();

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

			SEngineSystem::ExecuteAll();
		}
	}

	void StellarEngine::ShutdownEngine()
	{
		SEngineSystem::ReleaseAll();

		SAPIConfigurator::ShutdownAPI();
	}
}
