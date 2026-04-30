#ifndef _SE_CLEARPASS_H_
#define _SE_CLEARPASS_H_
#include "RenderPass.h"

namespace SE
{
	class GClearPass : public GRenderPass
	{
	public:
		GClearPass();
		GClearPass(const std::string& name, std::vector<unsigned int> multipleRenderTargetClearingList = {});
		GClearPass(const GClearPass& other);
		~GClearPass() override;

		void Execute() override;

	private:
		GResourcePackage ClearingFramebufferPackage;

		std::vector<unsigned int> MultipleRenderTargetClearingList;
	};
}

#endif