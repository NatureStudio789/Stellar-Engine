#include <Core.h>
#include "APIConfigurator.h"

namespace SE
{
	void SAPIConfigurator::InitializeAPI()
	{
		glfwInit();
	}

	void SAPIConfigurator::ShutdownAPI()
	{
		glfwTerminate();
	}
}
