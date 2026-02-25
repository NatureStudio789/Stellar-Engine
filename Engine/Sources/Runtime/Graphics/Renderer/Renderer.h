#ifndef _SE_RENDERER_H_
#define _SE_RENDERER_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Registry/Registry.h"
#include "RenderPass/RenderPass.h"

namespace SE
{
	class GRenderer : public SAddressable, public GContextDependent
	{
	public:
		GRenderer();
		virtual ~GRenderer();

		virtual void Execute();

	protected:
		void SetInflowTarget(const std::string& inflowName, const std::string& target);
		void AddGlobalInflow(std::shared_ptr<GInflow> inflow);
		void AddGlobalOutflow(std::shared_ptr<GOutflow> outflow);
		void AppendRenderPass(std::shared_ptr<GRenderPass> renderPass);

		std::shared_ptr<GRenderPass> GetRenderPass(const std::string& name);

		std::vector<std::shared_ptr<GRenderPass>> RenderPassList;

	private:
		void LinkPassInflows(std::shared_ptr<GRenderPass> renderPass);
		void LinkGlobalInflows();

		std::vector<std::shared_ptr<GInflow>> GlobalInflowList;
		std::vector<std::shared_ptr<GOutflow>> GlobalOutflowList;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GRenderer, RendererRegistry)
}

#endif