#ifndef _SE_STELLARENGINE_H_
#define _SE_STELLARENGINE_H_
#include "Core/APIConfigurator/APIConfigurator.h"
#include "Function/Input/Input.h"
#include "Function/Application/Application.h"
#include "Graphics/Context/GraphicsContext.h"
#include "Graphics/Framebuffer/Framebuffer.h"
#include "Function/Application/Event/EventProcesser.h"
#include "Core/EngineSystem/EngineSystem.h"

namespace SE
{
	class StellarEngine
	{
	public:
		StellarEngine();
		~StellarEngine();

		void InitializeEngine();

		void LaunchEngine();

		void ShutdownEngine();

		STELLAR_MAKE_SINGLETON(StellarEngine, Instance)

	private:
		std::shared_ptr<FEventProcesser> EngineEventProcesser;
	};
}

#endif