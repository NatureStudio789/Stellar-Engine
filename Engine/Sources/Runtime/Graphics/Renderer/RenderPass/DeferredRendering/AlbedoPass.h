#ifndef _SE_ALBEDOPASS_H_
#define _SE_ALBEDOPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GAlbedoPass : public GRenderQueuePass
	{
	public:
		GAlbedoPass();
		GAlbedoPass(const std::string& name);
		GAlbedoPass(const GAlbedoPass& other);
		~GAlbedoPass() override;

		void Initialize(const std::string& name);
		
		void Execute() override;

	private:
	};
}

#endif	