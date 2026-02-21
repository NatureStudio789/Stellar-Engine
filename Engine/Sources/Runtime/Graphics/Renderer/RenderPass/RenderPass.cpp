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
		SCommandListRegistry::Register(this->RenderPassName, this->PassCommandList);
	}

	GRenderPass::GRenderPass(const GRenderPass& other)
	{
		this->RenderPassName = other.RenderPassName;
	}

	GRenderPass::~GRenderPass()
	{

	}

	void GRenderPass::Execute()
	{

	}

	void GRenderPass::ActiveCommandList()
	{
		SCommandListRegistry::SetCurrentInstance(this->RenderPassName);
	}
}
