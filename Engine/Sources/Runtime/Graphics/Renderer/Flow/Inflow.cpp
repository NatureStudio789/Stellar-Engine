#include <Core.h>
#include "Outflow.h"
#include "Inflow.h"

namespace SE
{
	GInflow::GInflow(const std::string& name, GResourcePackage& package) : ResourcePackage(package)
	{
		CheckNameAvailable(name);

		this->InflowName = name;
	}

	GInflow::GInflow(const GInflow& other) : ResourcePackage(other.ResourcePackage)
	{
		this->InflowName = other.InflowName;
		this->TargetPassName = other.TargetPassName;
		this->LinkingOutflowName = other.LinkingOutflowName;
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
		this->ResourcePackage = outflow->GetResourcePackage();
	}

	const std::string& GInflow::GetName() const noexcept
	{
		return this->InflowName;
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
