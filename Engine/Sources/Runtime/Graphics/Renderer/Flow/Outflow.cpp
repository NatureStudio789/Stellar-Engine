#include <Core.h>
#include "Inflow.h"
#include "Outflow.h"

namespace SE
{
	GOutflow::GOutflow(const std::string& name, GResourcePackage package, bool isDynamic)
	{
		this->OutflowName = name;
		this->ResourcePackage = package;
		this->IsAvailable = true;
		this->IsDynamicFlowing = isDynamic;

		this->IsSourceOutflow = true;
		this->SourceInflowName = "";
	}

	GOutflow::GOutflow(const std::string& name, std::shared_ptr<GInflow> sourceInflow)
	{
		this->OutflowName = name;
		this->IsAvailable = false;
		this->IsDynamicFlowing = false;

		this->IsSourceOutflow = false;
		this->SourceInflowName = sourceInflow->GetName();
		sourceInflow->IsSource = true;
		sourceInflow->TargetOutflowOfSource = this->OutflowName;
	}

	GOutflow::GOutflow(const GOutflow& other)
	{
		this->OutflowName = other.OutflowName;
		this->ResourcePackage = other.ResourcePackage;
		this->BelongingPassName = other.BelongingPassName;
		this->IsAvailable = other.IsAvailable;
		this->IsDynamicFlowing = other.IsDynamicFlowing;

		this->IsSourceOutflow = other.IsSourceOutflow;
		this->SourceInflowName = other.SourceInflowName;
	}

	GOutflow::~GOutflow()
	{
		
	}

	bool GOutflow::GetAvailable() const noexcept
	{
		return this->IsAvailable;
	}

	const std::string& GOutflow::GetName() const noexcept
	{
		return this->OutflowName;
	}

	const bool& GOutflow::IsSource() const noexcept
	{
		return this->IsSourceOutflow;
	}

	GResourcePackage GOutflow::GetResourcePackage()
	{
		return this->ResourcePackage;
	}

	const std::string& GOutflow::GetBelongingPassName()
	{
		return this->BelongingPassName;
	}
}
