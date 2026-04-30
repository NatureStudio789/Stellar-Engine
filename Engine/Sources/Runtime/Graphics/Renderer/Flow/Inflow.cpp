#include <Core.h>
#include "Outflow.h"
#include "Inflow.h"

namespace SE
{
	GInflow::GInflow(const std::string& name, GResourcePackage& package) : ResourcePackage(package)
	{
		CheckNameAvailable(name);

		this->InflowName = name;
		this->IsLinked = false;
		this->IsSource = false;
	}

	GInflow::GInflow(const GInflow& other) : ResourcePackage(other.ResourcePackage)
	{
		this->InflowName = other.InflowName;
		this->IsLinked = other.IsLinked;
		this->BelongingPassName = other.BelongingPassName;

		this->TargetPassName = other.TargetPassName;
		this->LinkingOutflowName = other.LinkingOutflowName;

		this->IsSource = other.IsSource;
		this->TargetOutflowOfSource = other.TargetOutflowOfSource;
	}

	GInflow::~GInflow()
	{

	}

	void GInflow::SetLinkingTarget(std::string passName, std::string outflowName)
	{
		if (passName != "$")
		{
			CheckNameAvailable(passName);
		}
		CheckNameAvailable(outflowName);

		this->TargetPassName = passName;
		this->LinkingOutflowName = outflowName;
	}

	void GInflow::Apply(std::shared_ptr<GOutflow> outflow)
	{
		if (this->IsLinked)
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("This inflow named '{}' has already been linked!", this->InflowName));
		}

		this->ResourcePackage = outflow->ResourcePackage;
		this->IsLinked = true;
	}

	const std::string& GInflow::GetName() const noexcept
	{
		return this->InflowName;
	}

	const bool& GInflow::GetLinked() const noexcept
	{
		return this->IsLinked;
	}

	const std::string& GInflow::GetBelongingPassName() const noexcept
	{
		return this->BelongingPassName;
	}

	const std::string& GInflow::GetTargetPassName() const noexcept
	{
		return this->TargetPassName;
	}

	const std::string& GInflow::GetLinkingOutflowName() const noexcept
	{
		return this->LinkingOutflowName;
	}

	void GInflow::CheckNameAvailable(const std::string& name)
	{
		if (name.empty())
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Invalid Empty Name!");
		}

		const bool NameCharactersAvailable = std::all_of(name.begin(), name.end(), 
		[](char character)
		{
			return std::isalnum(character) || character == '_';
		});
		if (!NameCharactersAvailable || std::isdigit(name.front()))
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Invalid Characters in the name!");
		}
	}
}
