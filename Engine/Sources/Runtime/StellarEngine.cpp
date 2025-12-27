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
		SAPIInitializer::InitializeAPI();

		FWindow::Attribution MainWindowAttribution;
		MainWindowAttribution.SetDefaultSize();
		MainWindowAttribution.WindowTitle = "Stellar Engine";
		FApplication::Instance->InitializeApplication(MainWindowAttribution);
	}

	void StellarEngine::LaunchEngine()
	{
		while (FApplication::Instance->IsApplicationRunning())
		{
			FApplication::Instance->UpdateApplication();
		}
	}

	void StellarEngine::ShutdownEngine()
	{
		SAPIInitializer::ShutdownAPI();
	}
}
