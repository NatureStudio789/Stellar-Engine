#ifndef _SE_NORMALPASS_H_
#define _SE_NORMALPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GNormalPass : public GRenderQueuePass
	{
	public:
		GNormalPass();
		GNormalPass(const std::string& name);
		GNormalPass(const GNormalPass& other);
		~GNormalPass() override;

		void Initialize(const std::string& name);

		void Execute() override;
	};
}

#endif