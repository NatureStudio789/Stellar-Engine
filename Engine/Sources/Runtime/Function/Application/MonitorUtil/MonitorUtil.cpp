#include "MonitorUtil.h"

namespace SE
{
	SVector2 FMonitorUtil::GetMonitorResolution()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return SVector2((SFloat)MonitorVideoMode->width, (SFloat)MonitorVideoMode->height);
	}

	SInt FMonitorUtil::GetMonitorResolutionX()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return MonitorVideoMode->width;
	}

	SInt FMonitorUtil::GetMonitorResolutionY()
	{
		GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* MonitorVideoMode = glfwGetVideoMode(PrimaryMonitor);

		return MonitorVideoMode->height;
	}
}
