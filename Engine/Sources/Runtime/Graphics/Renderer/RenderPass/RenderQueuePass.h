#ifndef _SE_RENDERQUEUEPASS_H_
#define _SE_RENDERQUEUEPASS_H_
#include "ApplyPass.h"

namespace SE
{
	class GDrawTask;

	class GRenderQueuePass : public GApplyPass
	{
	public:
		using GApplyPass::GApplyPass;

		virtual void Accept(std::shared_ptr<GDrawTask> task);

		virtual void Execute() override;

	protected:
		std::vector<std::shared_ptr<GDrawTask>> TaskList;
	};
}

#endif	