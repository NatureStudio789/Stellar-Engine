#include <Core.h>
#include "Inflow.h"
#include "Outflow.h"

namespace SE
{
	GOutflow::GOutflow(const std::string& name, GResourcePackage package, bool isDynamic)
	{
		this->IsSourceOutflow = true;

		this->OutflowName = name;
		this->IsAvailable = true;
		this->IsDynamicFlowing = isDynamic;

		this->SourceInflowName = "";

		this->SetResourcePackage(package);
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

	void GOutflow::SetResourcePackage(GResourcePackage package)
	{
		if (!this->IsSourceOutflow)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "GOutflow::SetResourcePackage() is ONLY available for Source Outflow!");
		}

		this->ResourcePackage = package;
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
