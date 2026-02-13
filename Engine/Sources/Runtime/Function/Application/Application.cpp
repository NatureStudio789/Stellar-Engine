#include <Core.h>
#include "Application.h"

namespace SE
{
	STELLAR_FINE_SINGLETON(FApplication, Instance);


	void FApplication::InitializeApplication()
	{
		FWindow::Attribution MainWindowAttribution;
		MainWindowAttribution.WindowStyle = FWindow::SE_WINDOW_DEFAULT;
		MainWindowAttribution.WindowTitle = "Stellar Engine";
		this->MainWindow = FWindow::Create(MainWindowAttribution);

		SWindowRegistry::Register(SWindowRegistry::MainInstanceName, FApplication::Instance->GetMainWindow());
	}

	void FApplication::UpdateApplication()
	{
		this->MainWindow->ProcessMessage();
	}

	void FApplication::ShutdownApplication()
	{
		this->MainWindow.reset();

		
	}

	bool FApplication::IsApplicationRunning()
	{
		return this->MainWindow->IsWindowRunning();
	}

	std::shared_ptr<FWindow> FApplication::GetMainWindow()
	{
		return this->MainWindow;
	}
}
