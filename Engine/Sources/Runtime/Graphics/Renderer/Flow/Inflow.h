#ifndef _SE_INFLOW_H_
#define _SE_INFLOW_H_
#include "../Core/Creatable/Creatable.h"
#include "../ResourcePackage/ResourcePackage.h"

namespace SE
{
	class GOutflow;

	// The sink of the flowing resource.
	class GInflow : public SCreatable<GInflow>
	{
	public:
		GInflow(const std::string& name, GResourcePackage& package);
		GInflow(const GInflow& other);
		~GInflow();

		void SetLinkingTarget(std::string passName, std::string outflowName);
		// Apply the outflow which is in the pass to this inflow for resource package passing.
		void Apply(std::shared_ptr<GOutflow> outflow);

		const std::string& GetName() const noexcept;
		const std::string& GetTargetPassName() const noexcept;
		const std::string& GetLinkingOutflowName() const noexcept;

	private:
		static void CheckNameAvailable(const std::string& name);

		std::string InflowName;

		std::string TargetPassName;
		std::string LinkingOutflowName;

		GResourcePackage& ResourcePackage;
	};

	/* Description of Inflow and Outflow System :
		In general, the inflow get linked from the outflow of another pass. 
	The outflow doesn't link to the inflow proactively. 
		In a pass, the outflows and inflows will pass resource package with 
	each other.*/
}

#endif