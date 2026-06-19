#ifndef _SE_ROUGHNESSPASS_H_
#define _SE_ROUGHNESSPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GRoughnessPass : public GRenderQueuePass
	{
	public:
		GRoughnessPass();
		GRoughnessPass(const std::string& name);
		GRoughnessPass(const GRoughnessPass& other);
		~GRoughnessPass() override;

		void Initialize(const std::string& name);

		void Execute() override;
	};
}

#endif