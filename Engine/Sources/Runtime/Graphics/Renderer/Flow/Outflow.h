#ifndef _SE_OUTFLOW_H_
#define _SE_OUTFLOW_H_
#include "../../../Core/Creatable/Creatable.h"
#include "../ResourcePackage/ResourcePackage.h"

namespace SE
{
	class GInflow;
	
	// The source of the flowing resource.
	class GOutflow : public SCreatable<GOutflow>
	{
	public:
		GOutflow(const std::string& name, GResourcePackage package, bool isDynamic = false);
		GOutflow(const std::string& name, std::shared_ptr<GInflow> sourceInflow);
		GOutflow(const GOutflow& other);
		~GOutflow();

		bool GetAvailable() const noexcept;
		const std::string& GetName() const noexcept;
		/*Tag whether this outflow is a source, which means that it get its own
		resource package directly, not intermediately getting it from another inflow.*/
		const bool& IsSource() const noexcept;
		GResourcePackage GetResourcePackage();
		const std::string& GetBelongingPassName();

	private:
		std::string OutflowName;
		GResourcePackage ResourcePackage;
		std::string BelongingPassName;
		bool IsAvailable;
		bool IsDynamicFlowing;

		/*This attribution is to tag whether this outflow is a source, which means that it get its own 
		resource package directly, not intermediately getting it from another inflow.*/
		bool IsSourceOutflow;
		/*The name of specific inflow which offers the source of resource package.*/
		std::string SourceInflowName;

		friend class GRenderPass;
		friend class GInflow;
		friend class GFlowChain;
		friend class GRenderer;
	};
}

#endif