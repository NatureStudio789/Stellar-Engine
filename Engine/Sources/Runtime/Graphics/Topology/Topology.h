#ifndef _SE_TOPOLOGY_H_
#define _SE_TOPOLOGY_H_
#include "../../Core/Creatable/Creatable.h"
#include "../Applicable/Applicable.h"

namespace SE
{
	class GTopology : public GApplicable, public SCreatable<GTopology>
	{
	public:
		enum Type
		{
			SE_TOPOLOGY_NONE = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
			SE_TOPOLOGY_POINTLIST = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			SE_TOPOLOGY_LINELIST = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			SE_TOPOLOGY_LINESTRIP = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			SE_TOPOLOGY_TRIANGLELIST = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			SE_TOPOLOGY_TRIANGLESTRIP = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		};

	public:
		GTopology();
		GTopology(Type type);
		GTopology(const GTopology& other);
		~GTopology();

		void SetType(Type type);

		void Apply() override;

		const Type& GetType() const noexcept;

	private:
		Type TopologyType;
	};
}

#endif