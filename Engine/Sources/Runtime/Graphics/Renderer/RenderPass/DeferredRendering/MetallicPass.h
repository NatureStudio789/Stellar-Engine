#ifndef _SE_METALLICPASS_H_
#define _SE_METALLICPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GMetallicPass : public GRenderQueuePass
	{
	public:
		GMetallicPass();
		GMetallicPass(const std::string& name);
		GMetallicPass(const GMetallicPass& other);
		~GMetallicPass() override;

		void Initialize(const std::string& name);

		void Execute() override;

	private:

	};
}

#endif