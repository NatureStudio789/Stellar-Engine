#include <Core.h>
#include "../../Context/GraphicsContext.h"
#include "../Renderable/Renderable.h"
#include "DrawTask.h"

namespace SE
{
	GDrawTask::GDrawTask()
	{

	}

	GDrawTask::GDrawTask(std::shared_ptr<GRenderStage> renderStage, std::shared_ptr<GRenderable> renderable)
	{
		this->Initialize(renderStage, renderable);
	}

	GDrawTask::GDrawTask(const GDrawTask& other)
	{
		this->RenderStage = other.RenderStage;
		this->Renderable = other.Renderable;
	}

	GDrawTask::~GDrawTask()
	{

	}

	void GDrawTask::Initialize(std::shared_ptr<GRenderStage> renderStage, std::shared_ptr<GRenderable> renderable)
	{
		this->RenderStage = renderStage;
		this->Renderable = renderable;
	}

	void GDrawTask::Execute()
	{
		this->Renderable->Apply();
		this->RenderStage->Apply();

		if (this->Renderable->HasIndexBuffer())
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->DrawIndexedInstanced(
				this->Renderable->GetIndicesCount(), 1, 0, 0, 0);
		}
		else
		{
			SCommandListRegistry::GetCurrentInstance()->GetInstance()->DrawInstanced(
				this->Renderable->GetVerticesCount(), 1, 0, 0);
		}
	}
}
