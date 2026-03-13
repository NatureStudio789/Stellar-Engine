#ifndef _SE_APPLYPASS_H_
#define _SE_APPLYPASS_H_
#include "RenderPass.h"
#include "../../Applicable/Applicable.h"

namespace SE
{
	class GApplyPass : public GRenderPass
	{
	public:
		GApplyPass();
		GApplyPass(const std::string& name,
			std::vector<std::shared_ptr<GApplicable>> applicables = {});
		GApplyPass(const GApplyPass& other);
		virtual ~GApplyPass() override;

		void Initialize(const std::string& name,
			std::vector<std::shared_ptr<GApplicable>> applicables = {});
		void AddApplicable(std::shared_ptr<GApplicable> applicable);

		virtual void Apply();
		virtual void Execute() override;

	protected:
		GResourcePackage FramebufferPackage;

		std::vector<std::shared_ptr<GApplicable>> ApplicableList;
	};
}

#endif