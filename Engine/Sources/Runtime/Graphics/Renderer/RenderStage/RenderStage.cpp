#include <Core.h>
#include "../../../Core/MessageHandler/MessageHandler.h"
#include "../RenderPass/RenderQueuePass.h"
#include "../Renderer.h"
#include "RenderStage.h"
#include "../DrawTask/DrawTask.h"

namespace SE
{
	GRenderStage::GRenderStage()
	{
		this->ApplicableList.clear();

		this->TargetPassName = "";
		this->TargetPassList.clear();
	}

	GRenderStage::GRenderStage(const std::string& renderQueuePassName)
	{
		this->Initialize(renderQueuePassName);
	}

	GRenderStage::GRenderStage(const GRenderStage& other)
	{
		this->ApplicableList = other.ApplicableList;

		this->TargetPassName = other.TargetPassName;
		this->TargetPassList = other.TargetPassList;
	}

	GRenderStage::~GRenderStage()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();

		this->TargetPassName.clear();
		this->TargetPassList.clear();
	}

	void GRenderStage::Initialize(const std::string& renderQueuePassName)
	{
		this->TargetPassName = renderQueuePassName;
	}

	void GRenderStage::AddApplicable(std::shared_ptr<GApplicable> applicable)
	{
		this->ApplicableList.push_back(applicable);
	}

	void GRenderStage::SetParent(const GRenderable& renderable)
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->SetParent(renderable);
		}
	}

	void GRenderStage::Apply()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}
	}

	void GRenderStage::Submit(std::shared_ptr<GRenderable> renderable)
	{
		for (auto& targetPass : this->TargetPassList)
		{
			targetPass->Accept(GDrawTask::Create(std::make_shared<GRenderStage>(*this), renderable));
		}
	}

	void GRenderStage::Link(std::string rendererName)
	{
		auto& Renderer = SRendererRegistry::GetInstance(rendererName);

		if (auto pass = dynamic_cast<GRenderQueuePass*>(Renderer->GetRenderPass(this->TargetPassName).get()))
		{
			this->TargetPassList.push_back(pass);
		}
		else
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("The pass named '{}' is NOT a render queue pass in renderer named '{}'!",
					this->TargetPassName, rendererName));
		}
	}
}
