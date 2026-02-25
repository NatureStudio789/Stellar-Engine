#include <Core.h>
#include "RenderPass.h"

namespace SE
{
	GRenderPass::GRenderPass()
	{

	}

	GRenderPass::GRenderPass(const std::string& name)
	{
		this->RenderPassName = name;
		
		this->PassCommandList = GCommandList::Create(
			this->GetContext()->GetDevice(), GCommandList::SE_COMMAND_LIST_DIRECT);
		this->PassCommandList->SetName(this->RenderPassName);
		SCommandListRegistry::Register(this->PassCommandList);
	}

	GRenderPass::GRenderPass(const GRenderPass& other)
	{
		this->RenderPassName = other.RenderPassName;
	}

	GRenderPass::~GRenderPass()
	{

	}

	void GRenderPass::SetLinkage(const std::string& inflowName, const std::string& target)
	{
		auto Inflow = this->GetInflow(inflowName);

		auto SplitString = [](const std::string& text, const std::string& delim)
		{
			std::vector<std::string> strings;
			auto out = std::back_inserter(strings);

			if (delim.empty())
			{
				*out++ = text;
			}
			else
			{
				size_t a = 0, b = text.find(delim);
				for (; b != std::string::npos;
					a = b + delim.length(), b = text.find(delim, a))
				{
					*out++ = std::move(text.substr(a, b - a));
				}
				*out++ = std::move(text.substr(a, text.length() - a));
			}

			return strings;
		};
		std::vector<std::string> TargetSplit = SplitString(target, ".");
		if (TargetSplit.size() != (size_t)2)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Format of the target for linking is invalid!");
		}

		Inflow->SetLinkingTarget(TargetSplit[0], TargetSplit[1]);
	}

	void GRenderPass::Execute()
	{
		
	}

	const std::string& GRenderPass::GetName() const noexcept
	{
		return this->RenderPassName;
	}

	const std::vector<std::shared_ptr<GInflow>>& GRenderPass::GetInflowList() const noexcept
	{
		return this->InflowList;
	}

	std::shared_ptr<GInflow> GRenderPass::GetInflow(const std::string& name)
	{
		for (auto& inflow : this->InflowList)
		{
			if (inflow->GetName() == name)
			{
				return inflow;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics",
			std::format("No inflow named '{}' found in the {} Pass", name, this->RenderPassName));
	}

	std::shared_ptr<GOutflow> GRenderPass::GetOutflow(const std::string& name)
	{
		for (auto& outflow : this->OutflowList)
		{
			if (outflow->GetName() == name)
			{
				return outflow;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics",
			std::format("No outflow named '{}' found in the {} Pass", name, this->RenderPassName));
	}

	void GRenderPass::ActivateCommandList()
	{
		SCommandListRegistry::SetCurrentInstance(this->RenderPassName);
	}

	void GRenderPass::AddInflow(std::shared_ptr<GInflow> inflow)
	{
		for (auto& _inflow : this->InflowList)
		{
			if (_inflow->GetName() == inflow->GetName())
			{
				SMessageHandler::Instance->SetFatal("Graphics",
					std::format("There is already a inflow named '{}' in the {} Pass!", inflow->GetName(), this->RenderPassName));
				return;
			}
		}

		this->InflowList.push_back(inflow);
	}

	void GRenderPass::AddOutflow(std::shared_ptr<GOutflow> outflow)
	{
		for (auto& _outflow : this->OutflowList)
		{
			if (_outflow->GetName() == outflow->GetName())
			{
				SMessageHandler::Instance->SetFatal("Graphics",
					std::format("There is already a outflow named '{}' in the {} Pass!", outflow->GetName(), this->RenderPassName));
				return;
			}
		}

		this->OutflowList.push_back(outflow);
	}
}
