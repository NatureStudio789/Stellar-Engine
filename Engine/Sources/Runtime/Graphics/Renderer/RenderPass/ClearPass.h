#ifndef _SE_CLEARPASS_H_
#define _SE_CLEARPASS_H_
#include "RenderPass.h"

namespace SE
{
	class GClearPass : public GRenderPass
	{
	public:
		GClearPass();
		GClearPass(const std::string& name);
		GClearPass(const GClearPass& other);
		~GClearPass() override;

		void Execute() override;

	private:
		GResourcePackage ClearingFramebufferPackage;
	};
}

#endif