#ifndef _SE_RENDERSTAGE_H_
#define _SE_RENDERSTAGE_H_
#include "../../../Core/Creatable/Creatable.h"

#include "../../Applicable/Applicable.h"

namespace SE
{
	class GRenderQueuePass;
	class GRenderable;

	class GRenderStage : public SCreatable<GRenderStage>
	{
	public:
		GRenderStage();
		GRenderStage(const std::string& renderQueuePassName);
		GRenderStage(const GRenderStage& other);
		~GRenderStage();

		void Initialize(const std::string& renderQueuePassName);
		void AddApplicable(std::shared_ptr<GApplicable> applicable);

		void SetParent(const GRenderable& renderable);

		void Apply();
		void Submit(std::shared_ptr<GRenderable> renderable);

		void Link(std::string rendererName);

	private:
		std::vector<std::shared_ptr<GApplicable>> ApplicableList;

		std::string TargetPassName;
		std::vector<GRenderQueuePass*> TargetPassList;
	};
}

#endif