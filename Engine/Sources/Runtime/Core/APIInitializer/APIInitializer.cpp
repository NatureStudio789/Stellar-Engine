#include <Core.h>
#include "APIInitializer.h"

namespace SE
{
	void SAPIInitializer::InitializeAPI()
	{
		glfwInit();
	}

	void SAPIInitializer::ShutdownAPI()
	{
		glfwTerminate();
	}
}
