#include <Core.h>
#include "EditorRegistry.h"

#include "ViewportEditor/ViewportEditor.h"

namespace SE
{
	std::map<SUUID, std::shared_ptr<EEditor>> SEditorRegistry::RegisteredInstanceList;
	std::map<SUUID, bool> SEditorRegistry::EditorStateList;
	

	void SEditorRegistry::Register(std::shared_ptr<EEditor> instance, bool isActive)
	{
		if (RegisteredInstanceList.count(instance->GetUUID()))
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("The instance with id : '{}' has been registered in registry!", (const std::string&)instance->GetUUID()));
		}

		if (!instance->GetUUID().GetAvailable())
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("The instance which needs to be registered named '{}' is NOT AVAILABLE!", instance->GetName()));
		}

		RegisteredInstanceList[instance->GetUUID()] = instance;
		EditorStateList[instance->GetUUID()] = isActive;
	}

	std::shared_ptr<EEditor> SEditorRegistry::GetInstance(std::string name)
	{
		for (auto& [uuid, instance] : RegisteredInstanceList)
		{
			if (instance->GetName() == name)
			{
				return instance;
			}
		}

		SMessageHandler::Instance->SetFatal("Core",
			std::format("No instance named '{}' found in registry!", name));
		return null;
	}

	std::shared_ptr<EEditor> SEditorRegistry::GetInstance(SUUID uuid)
	{
		if (RegisteredInstanceList.count(uuid) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("No instance with id : '{}' found in registry!", (const std::string&)uuid));
		}

		return RegisteredInstanceList[uuid];
	}

	bool SEditorRegistry::HasInstance(std::string name)
	{
		for (auto& [uuid, instance] : RegisteredInstanceList)
		{
			if (instance->GetName() == name)
			{
				return true;
			}
		}

		return false;
	}

	bool SEditorRegistry::HasInstance(SUUID uuid)
	{
		return RegisteredInstanceList.count(uuid) == 1;
	}

	std::map<SUUID, std::shared_ptr<EEditor>> SEditorRegistry::GetInstanceList()
	{
		return RegisteredInstanceList;
	}

	void SEditorRegistry::SetEditorActive(const SUUID& uuid, bool isActive)
	{
		if (EditorStateList.count(uuid) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("No instance with id : '{}' found in registry!", (const std::string&)uuid));
		}

		EditorStateList[uuid] = isActive;
	}

	void SEditorRegistry::SetEditorActive(const std::string& name, bool isActive)
	{
		for (auto& [uuid, instance] : RegisteredInstanceList)
		{
			if (instance->GetName() == name)
			{
				EditorStateList[uuid] = isActive;
				return;
			}
		}

		SMessageHandler::Instance->SetFatal("Core",
			std::format("No instance named '{}' found in registry!", name));
	}

	bool SEditorRegistry::GetEditorActive(const SUUID& uuid)
	{
		if (EditorStateList.count(uuid) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("No instance with id : '{}' found in registry!", (const std::string&)uuid));
		}

		return EditorStateList[uuid];
	}

	bool SEditorRegistry::GetEditorActive(const std::string& name)
	{
		for (auto& [uuid, instance] : RegisteredInstanceList)
		{
			if (instance->GetName() == name)
			{
				return EditorStateList[uuid];
			}
		}

		SMessageHandler::Instance->SetFatal("Core",
			std::format("No instance named '{}' found in registry!", name));
		return false;
	}



	void SEditorRegistry::Initialize()
	{
		std::shared_ptr<EViewportEditor> ViewportEditor = std::make_shared<EViewportEditor>("Viewport");
		Register(ViewportEditor);
	}
}
