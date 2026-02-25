#ifndef _SE_RENDERENGINE_H_
#define _SE_RENDERENGINE_H_
#include "../Core/EngineSystem/Engine.h"

namespace SE
{
	class RenderEngine : public SEngine
	{
	public:
		RenderEngine();

		void Initialize() override;

		void Execute() override;

		void Release() override;

	private:
		std::shared_ptr<GGraphicsContext> MainGraphicsContext;
	};
}

#endif