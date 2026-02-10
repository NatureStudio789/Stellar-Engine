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
		SAPIInitializer::InitializeAPI();

		// Initialize application of engine.
		FWindow::Attribution MainWindowAttribution;
		MainWindowAttribution.WindowStyle = FWindow::SE_WINDOW_DEFAULT;
		MainWindowAttribution.WindowTitle = "Stellar Engine";
		FApplication::Instance->InitializeApplication(MainWindowAttribution);
		SWindowRegistry::Register(SWindowRegistry::MainInstanceName, FApplication::Instance->GetMainWindow());

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
		SAPIInitializer::ShutdownAPI();
	}
}
