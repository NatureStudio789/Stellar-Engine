#ifndef _SE_DRAWTASK_H_
#define _SE_DRAWTASK_H_
#include "../../../Core/Creatable/Creatable.h"

#include "../RenderStage/RenderStage.h"

namespace SE
{
	class GRenderable;

	class GDrawTask : public SCreatable<GDrawTask>
	{
	public:
		GDrawTask();
		GDrawTask(std::shared_ptr<GRenderStage> renderStage, std::shared_ptr<GRenderable> renderable);
		GDrawTask(const GDrawTask& other);
		~GDrawTask();

		void Initialize(std::shared_ptr<GRenderStage> renderStage, std::shared_ptr<GRenderable> renderable);

		void Execute();

	private:
		std::shared_ptr<GRenderStage> RenderStage;
		std::shared_ptr<GRenderable> Renderable;
	};
}

#endif