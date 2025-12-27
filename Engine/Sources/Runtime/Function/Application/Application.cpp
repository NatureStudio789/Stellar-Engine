#include "Application.h"

namespace SE
{
	STELLAR_FINE_SINGLETON(FApplication, Instance);


	void FApplication::InitializeApplication(const FWindow::Attribution& mainWindowAttribution)
	{
		this->MainWindow = std::make_shared<FWindow>(mainWindowAttribution);
	}

	void FApplication::UpdateApplication()
	{
		this->MainWindow->ProcessMessage();
	}

	void FApplication::ShutdownApplication()
	{
		this->MainWindow.reset();

		
	}

	SBool FApplication::IsApplicationRunning()
	{
		return this->MainWindow->IsWindowRunning();
	}

	std::shared_ptr<FWindow> FApplication::GetMainWindow()
	{
		return this->MainWindow;
	}
}
