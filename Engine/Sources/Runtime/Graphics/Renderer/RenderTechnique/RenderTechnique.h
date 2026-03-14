#ifndef _SE_RENDERTECHNIQUE_H_
#define _SE_RENDERTECHNIQUE_H_
#include "../RenderStage/RenderStage.h"

namespace SE
{
	class GRenderTechnique
	{
	public:
		GRenderTechnique();
		GRenderTechnique(const std::string& name, const std::string& channel, bool isActive = true);
		GRenderTechnique(const GRenderTechnique& other);
		~GRenderTechnique();

		void Initialize(const std::string& name, const std::string& channel, bool isActive = true);
		void AddRenderStage(std::shared_ptr<GRenderStage> renderStage);

		void SetParent(const GRenderable& parent);

		void SetActive(bool active);

		void Submit(std::shared_ptr<GRenderable> renderable, const std::string& channel);
		void Link(const std::string& rendererName);

		const bool& GetActive() const noexcept;

	private:
		std::string Name;
		std::string Channel;

		bool IsActive;

		std::vector<std::shared_ptr<GRenderStage>> RenderStageList;
	};
}

#endif