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
		SAPIInitializer::InitializeAPI();

		// Initialize application of engine.
		FWindow::Attribution MainWindowAttribution;
		MainWindowAttribution.SetDefaultSize();
		MainWindowAttribution.WindowTitle = "Stellar Engine";
		FApplication::Instance->InitializeApplication(MainWindowAttribution);

		// Initialize the input module.
		FInput::InitializeInput(FApplication::Instance->GetMainWindow()->GetWindowHandle()->Instance);
	}

	void StellarEngine::LaunchEngine()
	{
		while (FApplication::Instance->IsApplicationRunning())
		{
			FApplication::Instance->UpdateApplication();
			FInput::UpdateInput();
		}
	}

	void StellarEngine::ShutdownEngine()
	{
		SAPIInitializer::ShutdownAPI();
	}
}
