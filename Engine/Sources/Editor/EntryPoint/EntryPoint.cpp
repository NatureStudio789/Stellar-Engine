#include <Core.h>
#include "../../Runtime/Core/MessageHandler/Message.h"
#include "../../Runtime/StellarEngine.h"

int main(int argc, char* argv[])
{
	try
	{
		SE::StellarEngine::Instance->InitializeEngine();

		SE::StellarEngine::Instance->LaunchEngine();

		SE::StellarEngine::Instance->ShutdownEngine();
	}
	catch (const SE::SMessage& message)
	{
		(void)message;
		SE::SAPIConfigurator::ShutdownAPI();
		
		return -1;
	}

	return 0;
}
