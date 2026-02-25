#include <Core.h>
#include "Renderer.h"

namespace SE
{
	GRenderer::GRenderer()
	{

	}

	GRenderer::~GRenderer()
	{

	}

	void GRenderer::Execute()
	{
		std::vector<ID3D12GraphicsCommandList*> GraphicsCommandLists;

		for (auto& renderPass : this->RenderPassList)
		{
			this->LinkPassInflows(renderPass);

			renderPass->Execute();

			GraphicsCommandLists.push_back(
				renderPass->PassCommandList->GetInstance().Get());
		}

		this->LinkGlobalInflows();

		this->GetContext()->ExecuteCommandLists(GraphicsCommandLists);

		this->GetContext()->Flush();
	}

	void GRenderer::SetInflowTarget(const std::string& inflowName, const std::string& target)
	{
		const auto finder = [&inflowName](const std::shared_ptr<GInflow> inflow)
		{
			return inflow->GetName() == inflowName;
		};

		const auto i = std::find_if(this->GlobalInflowList.begin(), this->GlobalInflowList.end(), finder);
		if (i == this->GlobalInflowList.end())
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("No global inflow named '{}' found in renderer.", inflowName));
		}

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
		auto TargetSplit = SplitString(target, ".");
		if (TargetSplit.size() != (size_t)2)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Format of the target for linking is invalid!");
		}

		(*i)->SetLinkingTarget(TargetSplit[0], TargetSplit[1]);
	}

	void GRenderer::AddGlobalInflow(std::shared_ptr<GInflow> inflow)
	{
		for (auto& globalInflow : this->GlobalInflowList)
		{
			if (globalInflow->GetName() == inflow->GetName())
			{
				SMessageHandler::Instance->SetFatal("Graphics",
					std::format("There is already a global inflow named '{}' in renderer", inflow->GetName()));
				return;
			}
		}

		this->GlobalInflowList.push_back(inflow);
	}

	void GRenderer::AddGlobalOutflow(std::shared_ptr<GOutflow> outflow)
	{
		for (auto& globalOutflow : this->GlobalOutflowList)
		{
			if (globalOutflow->GetName() == outflow->GetName())
			{
				SMessageHandler::Instance->SetFatal("Graphics",
					std::format("There is already a global outflow named '{}' in renderer", outflow->GetName()));
				return;
			}
		}

		this->GlobalOutflowList.push_back(outflow);
	}

	void GRenderer::AppendRenderPass(std::shared_ptr<GRenderPass> renderPass)
	{
		for (auto& _renderPass : this->RenderPassList)
		{
			if (_renderPass->GetName() == renderPass->GetName())
			{
				SMessageHandler::Instance->SetFatal("Graphics",
					std::format("The render pass named '{}' already exists in render graph!", renderPass->GetName()));
				return;
			}
		}

		this->RenderPassList.push_back(renderPass);
	}

	std::shared_ptr<GRenderPass> GRenderer::GetRenderPass(const std::string& name)
	{
		for (auto& renderPass : this->RenderPassList)
		{
			if (renderPass->GetName() == name)
			{
				return renderPass;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics",
			std::format("No render pass named '{}' found in renderer!", name));
	}

	void GRenderer::LinkPassInflows(std::shared_ptr<GRenderPass> renderPass)
	{
		for (auto& inflow : renderPass->GetInflowList())
		{
			const std::string& InflowTargetPassName = inflow->GetTargetPassName();

			if (InflowTargetPassName == "$")
			{
				bool applied = false;
				for (auto& outflow : this->GlobalOutflowList)
				{
					if (outflow->GetName() == inflow->GetLinkingOutflowName())
					{
						inflow->Apply(outflow);
						applied = true;
						break;
					}
				}

				if (!applied)
				{
					SMessageHandler::Instance->SetFatal("Graphics",
						std::format("No global outflow named '{}' found in renderer", inflow->GetLinkingOutflowName()));
				}
			}
			else
			{
				auto RenderPass = this->GetRenderPass(InflowTargetPassName);

				auto outflow = RenderPass->GetOutflow(inflow->GetLinkingOutflowName());
				inflow->Apply(outflow);
			}
		}
	}

	void GRenderer::LinkGlobalInflows()
	{
		for (auto& inflow : this->GlobalInflowList)
		{
			const std::string& InflowTargetPassName = inflow->GetTargetPassName();
			auto RenderPass = this->GetRenderPass(InflowTargetPassName);
			auto outflow = RenderPass->GetOutflow(inflow->GetLinkingOutflowName());
			inflow->Apply(outflow);
		}
	}
}
