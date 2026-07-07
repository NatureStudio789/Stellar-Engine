#ifndef _SE_RENDERPASS_H_
#define _SE_RENDERPASS_H_
#include "../../Context/GraphicsContext.h"
#include "../../ContextDependent/ContextDependent.h"
#include "../Flow/FlowChain.h"
#include "../ResourcePackage/ResourcePackage.h"
#include "../../Camera/Camera.h"

namespace SE
{
	class GFramebuffer;
	class GRenderer;

	class GRenderPass : public GContextDependent
	{
	public:
		GRenderPass();
		GRenderPass(const std::string& name);
		GRenderPass(const GRenderPass& other);
		virtual ~GRenderPass();

		void SetLinkage(const std::string& inflowName, const std::string& target);

		virtual void Execute();

		const std::string& GetName() const noexcept;
		const std::vector<std::shared_ptr<GInflow>>& GetInflowList() const noexcept;
		std::shared_ptr<GInflow> GetInflow(const std::string& name);
		std::shared_ptr<GOutflow> GetOutflow(const std::string& name);

	protected:
		void AddInflow(std::shared_ptr<GInflow> inflow);
		void AddOutflow(std::shared_ptr<GOutflow> outflow);

		virtual void SetParent(const GRenderer* parent);

		std::shared_ptr<GFramebuffer> GetFramebufferInstance(const GResourcePackage& package);

		std::string RenderPassName;

		std::vector<std::shared_ptr<GInflow>> InflowList;
		std::vector<std::shared_ptr<GOutflow>> OutflowList;

		friend class GRenderer;
	};
}

#endif