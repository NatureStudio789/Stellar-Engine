#ifndef _SE_REGISTRY_H_
#define _SE_REGISTRY_H_
#include "../UUID/UUID.h"

namespace SE
{
	template<typename RegType>
	class SRegistry
	{
	public:
		static void Register(std::shared_ptr<RegType> instance);

		static std::shared_ptr<RegType> GetInstance(std::string name);
		static std::shared_ptr<RegType> GetInstance(SUUID uuid);

		static std::string MainInstanceName;

	protected:
		static std::map<SUUID, std::shared_ptr<RegType>> RegisteredInstanceList;
	};

	template<typename RegType>
	std::string SRegistry<RegType>::MainInstanceName = "Main";
	template<typename RegType>
	std::map<SUUID, std::shared_ptr<RegType>> SRegistry<RegType>::RegisteredInstanceList;

	template<typename RegType>
	inline void SRegistry<RegType>::Register(std::shared_ptr<RegType> instance)
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
	}

	template<typename RegType>
	inline std::shared_ptr<RegType> SRegistry<RegType>::GetInstance(std::string name)
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

	template<typename RegType>
	inline std::shared_ptr<RegType> SRegistry<RegType>::GetInstance(SUUID uuid)
	{
		if (RegisteredInstanceList.count(uuid) < 0)
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("No instance with id : '{}' found in registry!", (std::string)uuid));
		}

		return RegisteredInstanceList[uuid];
	}

#define STELLAR_MAKE_DEFAULT_REGISTRY(Type, ClassName)\
	class S##ClassName : public SRegistry<Type>\
	{\
	public:\
		static std::shared_ptr<Type> GetMainInstance()\
		{\
			return GetInstance(MainInstanceName);\
		}\
	};

#define STELLAR_MAKE_EXECUTABLE_REGISTRY(Type, ClassName)\
	class S##ClassName : public SRegistry<Type>\
	{\
	public:\
		static void SetCurrentInstance(std::string instanceName)\
		{\
			CurrentInstanceName = instanceName;\
		}\
		\
		static std::shared_ptr<Type> GetCurrentInstance()\
		{\
			return GetInstance(CurrentInstanceName);\
		}\
		\
	private:\
		static std::string CurrentInstanceName;\
	};\
	\
	inline std::string S##ClassName::CurrentInstanceName;
}

#endif