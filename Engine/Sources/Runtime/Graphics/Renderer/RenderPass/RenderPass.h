#ifndef _SE_RENDERPASS_H_
#define _SE_RENDERPASS_H_
#include "../../Context/GraphicsContext.h"
#include "../../ContextDependent/ContextDependent.h"
#include "../Flow/Inflow.h"
#include "../Flow/Outflow.h"
#include "../ResourcePackage/ResourcePackage.h"

namespace SE
{
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
		// This function must be called in any execution function.
		void ActivateCommandList();

		void AddInflow(std::shared_ptr<GInflow> inflow);
		void AddOutflow(std::shared_ptr<GOutflow> outflow);

		std::string RenderPassName;

		std::shared_ptr<GCommandList> PassCommandList;

		std::vector<std::shared_ptr<GInflow>> InflowList;
		std::vector<std::shared_ptr<GOutflow>> OutflowList;

		friend class GRenderer;
	};
}

#endif