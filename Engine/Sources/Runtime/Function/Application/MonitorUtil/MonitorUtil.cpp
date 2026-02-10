#include <Core.h>
#include "MonitorUtil.h"

namespace SE
{
	glm::uvec2 FMonitorUtil::GetMonitorResolution()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return glm::uvec2(MonitorVideoMode->width, MonitorVideoMode->height);
	}

	int FMonitorUtil::GetMonitorResolutionX()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return MonitorVideoMode->width;
	}

	int FMonitorUtil::GetMonitorResolutionY()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return MonitorVideoMode->height;
	}
}
