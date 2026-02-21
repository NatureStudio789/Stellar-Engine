#ifndef _SE_RENDERER_H_
#define _SE_RENDERER_H_
#include "RenderPass/RenderPass.h"

namespace SE
{
	class GRenderer : public GContextDependent
	{
	public:
		GRenderer();
		virtual ~GRenderer();

		virtual void Execute();

	protected:
		std::string RendererName;

		std::vector<std::shared_ptr<GRenderPass>> RenderPassList;
	};
}

#endif