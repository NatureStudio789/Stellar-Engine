#include <Core.h>
#include "../MessageHandler/MessageHandler.h"
#include "Engine.h"
#include "EngineSystem.h"

namespace SE
{
	void SEngineSystem::InitializeAll()
	{
		for (auto& [name, engine] : GetEngineList())
		{
			engine->Initialize();
		}
	}

	void SEngineSystem::ExecuteAll()
	{
		for (auto& [name, engine] : GetEngineList())
		{
			engine->Execute();
		}
	}

	void SEngineSystem::ReleaseAll()
	{
		for (auto& [name, engine] : GetEngineList())
		{
			engine->Release();
		}
	}

	void SEngineSystem::RegisterEngine(std::unique_ptr<SEngine> engine)
	{
		if (GetEngineList().count(engine->EngineName))
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("The engine named '{}' already exists in the system!", engine->EngineName));
		}

		GetEngineList()[engine->EngineName] = std::move(engine);
	}

	std::map<std::string, std::unique_ptr<SEngine>>& SEngineSystem::GetEngineList()
	{
		static std::map<std::string, std::unique_ptr<SEngine>> EngineList;
		return EngineList;
	}
}
