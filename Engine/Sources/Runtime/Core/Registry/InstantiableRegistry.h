#ifndef _SE_INSTANTIABLEREGISTRY_H_
#define _SE_INSTANTIABLEREGISTRY_H_
#include "../UUID/UUID.h"

namespace SE
{
	template<typename RegType>
	class SInstantiableRegistry
	{
	public:
		SInstantiableRegistry();
		SInstantiableRegistry(const SInstantiableRegistry& other);
		virtual ~SInstantiableRegistry();

		virtual void Register(std::shared_ptr<RegType> instance);
		virtual void Unregister(std::shared_ptr<RegType> instance);
		virtual void Unregister(std::string name);
		virtual void Unregister(SUUID uuid);

		std::shared_ptr<RegType> GetInstance(std::string name);
		std::shared_ptr<RegType> GetInstance(SUUID uuid);

		bool HasInstance(std::string name);
		bool HasInstance(SUUID uuid);

		std::map<SUUID, std::shared_ptr<RegType>> GetInstanceList();

	protected:
		std::map<SUUID, std::shared_ptr<RegType>> RegisteredInstanceList;
	};

	template<typename RegType>
	inline SInstantiableRegistry<RegType>::SInstantiableRegistry()
	{

	}

	template<typename RegType>
	inline SInstantiableRegistry<RegType>::SInstantiableRegistry(const SInstantiableRegistry& other)
	{
		this->RegisteredInstanceList = other.RegisteredInstanceList;
	}

	template<typename RegType>
	inline SInstantiableRegistry<RegType>::~SInstantiableRegistry()
	{
		this->RegisteredInstanceList.clear();
	}

	template<typename RegType>
	inline void SInstantiableRegistry<RegType>::Register(std::shared_ptr<RegType> instance)
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
	inline void SInstantiableRegistry<RegType>::Unregister(std::shared_ptr<RegType> instance)
	{
		SUUID RemoveInstanceKey;
		bool Found = false;
		for (auto& [uuid, _instance] : RegisteredInstanceList)
		{
			if (_instance == instance)
			{
				RemoveInstanceKey = uuid;
				Found = true;
				break;
			}
		}

		if (Found)
		{
			RegisteredInstanceList.erase(RemoveInstanceKey);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Core", "Failed to unregister! No available instance matched!");
		}
	}

	template<typename RegType>
	inline void SInstantiableRegistry<RegType>::Unregister(std::string name)
	{
		SUUID RemoveInstanceKey;
		bool Found = false;
		for (auto& [uuid, _instance] : RegisteredInstanceList)
		{
			if (_instance->GetName() == name)
			{
				RemoveInstanceKey = uuid;
				Found = true;
				break;
			}
		}

		if (Found)
		{
			RegisteredInstanceList.erase(RemoveInstanceKey);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Core", "Failed to unregister! No available instance matched!");
		}
	}

	template<typename RegType>
	inline void SInstantiableRegistry<RegType>::Unregister(SUUID uuid)
	{
		RegisteredInstanceList.erase(uuid);
	}

	template<typename RegType>
	inline std::shared_ptr<RegType> SInstantiableRegistry<RegType>::GetInstance(std::string name)
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
	inline std::shared_ptr<RegType> SInstantiableRegistry<RegType>::GetInstance(SUUID uuid)
	{
		if (RegisteredInstanceList.count(uuid) <= 0)
		{
			SMessageHandler::Instance->SetFatal("Core",
				std::format("No instance with id : '{}' found in registry!", (const std::string&)uuid));
		}

		return RegisteredInstanceList[uuid];
	}

	template<typename RegType>
	inline bool SInstantiableRegistry<RegType>::HasInstance(std::string name)
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

	template<typename RegType>
	inline bool SInstantiableRegistry<RegType>::HasInstance(SUUID uuid)
	{
		return RegisteredInstanceList.count(uuid) == 1;
	}

	template<typename RegType>
	inline std::map<SUUID, std::shared_ptr<RegType>> SInstantiableRegistry<RegType>::GetInstanceList()
	{
		return this->RegisteredInstanceList;
	}
}

#endif