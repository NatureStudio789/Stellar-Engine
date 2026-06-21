#ifndef _SE_POSITIONPASS_H_
#define _SE_POSITIONPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GPositionPass : public GRenderQueuePass
	{
	public:
		GPositionPass();
		GPositionPass(const std::string& name);
		GPositionPass(const GPositionPass& other);
		~GPositionPass() override;

		void Initialize(const std::string& name);

		void Execute() override;
	};
}

#endif