#include <Core.h>
#include "../../Framebuffer/Framebuffer.h"
#include "ApplyPass.h"

namespace SE
{
	GApplyPass::GApplyPass() : GRenderPass()
	{

	}

	GApplyPass::GApplyPass(const std::string& name, std::vector<std::shared_ptr<GApplicable>> applicables) : GRenderPass()
	{
		this->Initialize(name, applicables);
	}

	GApplyPass::GApplyPass(const GApplyPass& other) : GRenderPass(other)
	{
		this->FramebufferPackage = other.FramebufferPackage;
		this->ApplicableList = other.ApplicableList;
	}

	GApplyPass::~GApplyPass()
	{
		this->ApplicableList.clear();
	}

	void GApplyPass::Initialize(const std::string& name, std::vector<std::shared_ptr<GApplicable>> applicables)
	{
		this->RenderPassName = name;

		this->ApplicableList = applicables;
	}

	void GApplyPass::AddApplicable(std::shared_ptr<GApplicable> applicable)
	{
		this->ApplicableList.push_back(applicable);
	}

	void GApplyPass::Apply()
	{
		if (SFramebufferRegistry::HasInstance(this->FramebufferPackage.GetResourceIdentifier().GetUUID()))
		{
			this->GetFramebufferInstance(this->FramebufferPackage)->Apply();
		}

		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}
	}

	void GApplyPass::Execute()
	{

	}
}
