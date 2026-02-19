#ifndef _SE_STELLARENGINE_H_
#define _SE_STELLARENGINE_H_
#include "Core/APIConfigurator/APIConfigurator.h"
#include "Function/Input/Input.h"
#include "Function/Application/Application.h"
#include "Graphics/Context/GraphicsContext.h"
#include "Graphics/Framebuffer/Framebuffer.h"

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

		STELLAR_MAKE_SINGLETON(StellarEngine, Instance);

	private:
		std::shared_ptr<GGraphicsContext> MainGraphicsContext;
		std::shared_ptr<GFramebuffer> TestFramebuffer;
	};
}

#endif