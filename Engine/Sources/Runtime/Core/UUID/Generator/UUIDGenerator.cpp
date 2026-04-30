#include <Core.h>
#include "UUIDGenerator.h"

namespace SE
{
	std::string SUUIDGenerator::Generate()
	{
		UUID Uuid;
		RPC_STATUS Status = UuidCreate(&Uuid);
		if (Status != RPC_S_OK && Status != RPC_S_UUID_LOCAL_ONLY)
		{
			SMessageHandler::Instance->SetFatal("Core", "Failed to create UUID!");
		}

		RPC_CSTR UuidString = null;
		Status = UuidToStringA(&Uuid, &UuidString);
		if (Status != RPC_S_OK)
		{
			SMessageHandler::Instance->SetFatal("Core", "Failed to convert UUID to string!");
		}

		std::string Result(reinterpret_cast<const char*>(UuidString));

		RpcStringFreeA(&UuidString);
		std::cout << Result << std::endl;

		return Result;
	}
}
