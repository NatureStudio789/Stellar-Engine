#ifndef _SE_ENGINE_H_
#define _SE_ENGINE_H_
#include "EngineSystem.h"

namespace SE
{
	class SEngine
	{
	public:
		virtual void Initialize() {}

		virtual void Execute() {}

		virtual void Release() {}

	protected:
		std::string EngineName;

		friend class SEngineSystem;
	};

#define STELLAR_REGISTER_ENGINE(ClassName)\
	struct Register##ClassName\
	{\
		Register##ClassName()\
		{\
			SEngineSystem::RegisterEngine(std::move(std::make_unique<ClassName>()));\
		}\
	} Register##ClassName##Instance;
}

#endif