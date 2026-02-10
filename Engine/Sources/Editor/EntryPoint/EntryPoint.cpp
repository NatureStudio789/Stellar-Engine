#include <Core.h>
#include "../../Runtime/StellarEngine.h"

int main(int argc, char* argv[])
{
	SE::StellarEngine::Instance->InitializeEngine();

	SE::StellarEngine::Instance->LaunchEngine();

	SE::StellarEngine::Instance->ShutdownEngine();

	return 0;
}
