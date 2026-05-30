#include <Core.h>
#include "../Renderer.h"
#include "../../Framebuffer/Framebuffer.h"
#include "RenderQueuePass.h"

namespace SE
{
	void GRenderQueuePass::Accept(std::shared_ptr<GDrawTask> task)
	{
		this->TaskList.push_back(task);
	}

	void GRenderQueuePass::Apply()
	{
		GApplyPass::Apply();

		SCameraRegistry::ApplyCurrentInstance();
	}

	void GRenderQueuePass::Execute()
	{
		auto& Framebuffer = this->GetFramebufferInstance(this->FramebufferPackage);
		Framebuffer->Begin();

		this->GetContext()->ApplyDescriptorHeaps();
		this->Apply();

		for (auto& task : this->TaskList)
		{
			task->Execute();
		}

		Framebuffer->End();

		this->TaskList.clear();
	}
}
