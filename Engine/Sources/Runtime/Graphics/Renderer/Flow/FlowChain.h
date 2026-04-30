#ifndef _SE_FLOWCHAIN_H_
#define _SE_FLOWCHAIN_H_
#include "Inflow.h"
#include "Outflow.h"

namespace SE
{
	class GFlowChain
	{
	public:
		GFlowChain();
		GFlowChain(const GFlowChain& other);
		~GFlowChain();

		void Link();

		const bool& IsChainDynamic() const noexcept;

	private:
		/* The flow list stores a list of pairs, which involve one outflow and one inflow,
		these two is for resource passing between different pass.*/
		std::vector<std::pair<std::shared_ptr<GOutflow>, std::shared_ptr<GInflow>>> FlowList;
		bool IsDynamic;

		friend class GRenderer;
	};
}

#endif