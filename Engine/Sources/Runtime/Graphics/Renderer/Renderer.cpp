#include <Core.h>
#include "Renderer.h"

namespace SE
{
	GRenderer::GRenderer()
	{

	}

	GRenderer::GRenderer(const std::string& name)
	{
		this->SetName(name);

		this->RendererCommandList = GCommandList::Create(
			this->GetContext()->GetDevice(), GCommandList::SE_COMMAND_LIST_DIRECT);
		this->RendererCommandList->SetName(this->GetName());
		SCommandListRegistry::Register(this->RendererCommandList);
	}

	GRenderer::~GRenderer()
	{

	}

	void GRenderer::SetMainCamera(const std::string& name)
	{
		if (!SCameraRegistry::HasInstance(name))
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("No available camera named '{}' in registry!", name));
		}

		this->MainCameraName = name;
	}

	void GRenderer::Compile()
	{
		std::map<std::string, std::vector<std::shared_ptr<GInflow>>> TargetPassOrderedInflowList;
		std::map<std::string, std::shared_ptr<GOutflow>> PassSourceOutflowList;
		for (auto& pass : this->RenderPassList)
		{
			for (auto& inflow : pass->InflowList)
			{
				TargetPassOrderedInflowList[inflow->GetTargetPassName()].push_back(inflow);
			}

			for (auto& outflow : pass->OutflowList)
			{
				if (outflow->IsSource())
				{
					PassSourceOutflowList[outflow->BelongingPassName + "." + outflow->GetName()] = outflow;
				}
			}
		}

		for (auto& globalOutflow : this->GlobalOutflowList)
		{
			std::shared_ptr<GFlowChain> FlowChain = std::make_shared<GFlowChain>();
			FlowChain->IsDynamic = globalOutflow->IsDynamicFlowing;

			std::pair<std::shared_ptr<GOutflow>, std::shared_ptr<GInflow>> FlowNode;
			FlowNode.first = globalOutflow;

			for (auto& inflow : TargetPassOrderedInflowList["$"])
			{
				if (inflow->GetLinkingOutflowName() == globalOutflow->GetName())
				{
					FlowNode.second = inflow;
					FlowChain->FlowList.push_back(FlowNode);

					// Iterator the following linking nodes.
					if (inflow->IsSource)
					{
						this->BuildFlowChain(FlowChain, this->GetRenderPass(inflow->BelongingPassName)->GetOutflow(inflow->TargetOutflowOfSource), 
							TargetPassOrderedInflowList);
					}

					break;
				}
			}

			if (FlowChain->IsDynamic)
			{
				this->DynamicFlowChainList.push_back(FlowChain);
			}
			else
			{
				this->StaticFlowChainList.push_back(FlowChain);
			}
		}

		for (auto& [name, outflow] : PassSourceOutflowList)
		{
			std::shared_ptr<GFlowChain> FlowChain = std::make_shared<GFlowChain>();
			FlowChain->IsDynamic = outflow->IsDynamicFlowing;

			this->BuildFlowChain(FlowChain, outflow, TargetPassOrderedInflowList);

			if (FlowChain->IsDynamic)
			{
				this->DynamicFlowChainList.push_back(FlowChain);
			}
			else
			{
				this->StaticFlowChainList.push_back(FlowChain);
			}
		}

		for (auto& staticFlowChain : this->StaticFlowChainList)
		{
			staticFlowChain->Link();
		}
		for (auto& dynamicFlowChain : this->DynamicFlowChainList)
		{
			dynamicFlowChain->Link();
		}

		this->IsCompiled = true;
	}

	void GRenderer::BuildFlowChain(std::shared_ptr<GFlowChain> flowChain, std::shared_ptr<GOutflow> outflow,
		std::map<std::string, std::vector<std::shared_ptr<GInflow>>> targetPassOrderedInflowList)
	{
		std::pair<std::shared_ptr<GOutflow>, std::shared_ptr<GInflow>> FlowNode;
		FlowNode.first = outflow;

		for (auto& inflow : targetPassOrderedInflowList[outflow->BelongingPassName])
		{
			if (inflow->GetLinkingOutflowName() == outflow->GetName())
			{
				FlowNode.second = inflow;
				flowChain->FlowList.push_back(FlowNode);

				// Iterator the following linking nodes.
				if (inflow->IsSource)
				{
					this->BuildFlowChain(flowChain, this->GetRenderPass(inflow->BelongingPassName)->GetOutflow(inflow->TargetOutflowOfSource), 
						targetPassOrderedInflowList);
				}

				break;
			}
		}
	}

	void GRenderer::Execute()
	{
		if (!this->IsCompiled)
		{
			SMessageHandler::Instance->SetFatal("Graphics", std::format("The renderer named '{}' is NOT compiled before execution!", this->GetName()));
		}

		SCommandListRegistry::SetCurrentInstance(this->GetName());
		SCameraRegistry::SetCurrentInstance(this->MainCameraName);

		this->RendererCommandList->Open();

		for (auto& renderPass : this->RenderPassList)
		{
			renderPass->Execute();
		}

		this->RendererCommandList->Close();

		for (auto& dynamicFlowChain : this->DynamicFlowChainList)
		{
			dynamicFlowChain->Link();
		}
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

		inflow->BelongingPassName = "$";
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

		outflow->BelongingPassName = "$";
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

		renderPass->SetParent(this);
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
		return null;
	}

	void SRendererRegistry::CompileAllRenderer()
	{
		for (auto& [name, renderer] : RegisteredInstanceList)
		{
			renderer->Compile();
		}
	}
}
