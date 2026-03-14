#include <Core.h>
#include "RenderQueuePass.h"

namespace SE
{
	void GRenderQueuePass::Accept(std::shared_ptr<GDrawTask> task)
	{
		this->TaskList.push_back(task);
	}

	void GRenderQueuePass::Execute()
	{
		SCommandListRegistry::GetCurrentInstance()->Open();
		this->GetContext()->ApplyDescriptorHeaps();

		this->Apply();

		for (auto& task : this->TaskList)
		{
			task->Execute();
		}

		SCommandListRegistry::GetCurrentInstance()->Close();

		this->TaskList.clear();
	}
}
