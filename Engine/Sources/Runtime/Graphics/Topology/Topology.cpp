#include <Core.h>
#include "../CommandList/CommandList.h"
#include "Topology.h"

namespace SE
{
	GTopology::GTopology() : GApplicable()
	{
		this->TopologyType = SE_TOPOLOGY_NONE;
	}

	GTopology::GTopology(Type type)
	{
		this->SetType(type);
	}

	GTopology::GTopology(const GTopology& other)
	{
		this->TopologyType = other.TopologyType;
	}

	GTopology::~GTopology()
	{
		this->TopologyType = SE_TOPOLOGY_NONE;
	}

	void GTopology::SetType(Type type)
	{
		this->TopologyType = type;
	}

	void GTopology::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)this->TopologyType);
	}

	const GTopology::Type& GTopology::GetType() const noexcept
	{
		return this->TopologyType;
	}
}
