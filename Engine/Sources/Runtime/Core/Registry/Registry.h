#ifndef _SE_REGISTRY_H_
#define _SE_REGISTRY_H_

namespace SE
{
	template<typename RegType>
	class SRegistry
	{
	public:
		static void Register(std::string name, std::shared_ptr<RegType> instance);

		static std::shared_ptr<RegType> GetInstance(std::string name);

		static std::string MainInstanceName;

	protected:
		static std::map<std::string, std::shared_ptr<RegType>> RegisteredInstanceList;
	};

	template<typename RegType>
	std::string SRegistry<RegType>::MainInstanceName = "Main";
	template<typename RegType>
	std::map<std::string, std::shared_ptr<RegType>> SRegistry<RegType>::RegisteredInstanceList;

	template<typename RegType>
	inline void SRegistry<RegType>::Register(std::string name, std::shared_ptr<RegType> instance)
	{
		if (RegisteredInstanceList.count(name))
		{
			// TO DO: Throw exeception.
		}

		RegisteredInstanceList[name] = instance;
	}

	template<typename RegType>
	inline std::shared_ptr<RegType> SRegistry<RegType>::GetInstance(std::string name)
	{
		if (RegisteredInstanceList.count(name) < 0)
		{
			// TO DO: Throw exeception.
		}

		return RegisteredInstanceList[name];
	}

#define STELLAR_MAKE_DEFAULT_REGISTRY(Type, ClassName)\
	class S##ClassName : public SRegistry<Type>\
	{\
	public:\
		static std::shared_ptr<Type> GetMainInstance()\
		{\
			return RegisteredInstanceList[MainInstanceName];\
		}\
	};
}

#endif