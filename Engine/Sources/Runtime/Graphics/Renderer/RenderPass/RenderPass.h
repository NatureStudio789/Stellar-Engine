#ifndef _SE_RENDERPASS_H_
#define _SE_RENDERPASS_H_
#include "../../Context/GraphicsContext.h"
#include "../../ContextDependent/ContextDependent.h"

namespace SE
{
	class GRenderPass : public GContextDependent
	{
	public:
		GRenderPass();
		GRenderPass(const std::string& name);
		GRenderPass(const GRenderPass& other);
		virtual ~GRenderPass();

		virtual void Execute();

	protected:
		// This function must be called in any execution function.
		void ActivateCommandList();

		std::string RenderPassName;

		std::shared_ptr<GCommandList> PassCommandList;

		friend class GRenderer;
	};
}

#endif