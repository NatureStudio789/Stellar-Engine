#ifndef _SE_OUTFLOW_H_
#define _SE_OUTFLOW_H_
#include "../../../Core/Creatable/Creatable.h"
#include "../ResourcePackage/ResourcePackage.h"

namespace SE
{
	// The source of the flowing resource.
	class GOutflow : public SCreatable<GOutflow>
	{
	public:
		GOutflow(const std::string& name, GResourcePackage& package);
		GOutflow(const GOutflow& other);
		~GOutflow();

		const std::string& GetName() const noexcept;
		GResourcePackage& GetResourcePackage();

	private:
		std::string OutflowName;
		GResourcePackage& ResourcePackage;
	};
}

#endif