#include <Core.h>
#include "APIConfigurator.h"

namespace SE
{
	void SAPIConfigurator::InitializeAPI()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef SE_PLATFORM_MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	}

	void SAPIConfigurator::ShutdownAPI()
	{
		glfwTerminate();
	}
}
