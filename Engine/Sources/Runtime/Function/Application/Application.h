#ifndef _SE_APPLICATION_H_
#define _SE_APPLICATION_H_
#include "Window/Window.h"

namespace SE
{
	class FApplication
	{
	public:
		void InitializeApplication(const FWindow::Attribution& mainWindowAttribution);

		void UpdateApplication();

		void ShutdownApplication();

		bool IsApplicationRunning();

		std::shared_ptr<FWindow> GetMainWindow();

		STELLAR_MAKE_SINGLETON(FApplication, Instance);

	private:
		std::shared_ptr<FWindow> MainWindow;
	};
}

#endif