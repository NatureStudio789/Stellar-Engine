#include <Core.h>
#include "../../../Core/MessageHandler/MessageHandler.h"
#include "RenderTechnique.h"

namespace SE
{
	GRenderTechnique::GRenderTechnique()
	{
		this->Name = "";
		this->Channel = "";

		this->IsActive = false;

		this->RenderStageList.clear();
	}

	GRenderTechnique::GRenderTechnique(const std::string& name, const std::string& channel, bool isActive)
	{
		this->Initialize(name, channel, isActive);
	}

	GRenderTechnique::GRenderTechnique(const GRenderTechnique & other)
	{
		this->Name = other.Name;
		this->Channel = other.Channel;

		this->IsActive = other.IsActive;

		this->RenderStageList = other.RenderStageList;
	}

	GRenderTechnique::~GRenderTechnique()
	{
		this->Name.clear();
		this->Channel.clear();

		this->IsActive = false;

		for (auto& renderStage : this->RenderStageList)
		{
			renderStage.reset();
		}
		this->RenderStageList.clear();
	}

	void GRenderTechnique::Initialize(const std::string& name, const std::string & channel, bool isActive)
	{
		this->Name = name;
		this->Channel = channel;

		this->IsActive = isActive;
	}

	void GRenderTechnique::AddRenderStage(std::shared_ptr<GRenderStage> renderStage)
	{
		this->RenderStageList.push_back(renderStage);
	}

	void GRenderTechnique::SetParent(const GRenderable& parent)
	{
		for (auto& renderStage : this->RenderStageList)
		{
			renderStage->SetParent(parent);
		}
	}

	void GRenderTechnique::SetActive(bool active)
	{
		this->IsActive = active;
	}

	void GRenderTechnique::Submit(std::shared_ptr<GRenderable> renderable, const std::string & channel)
	{
		if (this->IsActive && this->Channel == channel)
		{
			for (auto& renderStage : this->RenderStageList)
			{
				renderStage->Submit(renderable);
			}
		}
	}

	void GRenderTechnique::Link(const std::string & rendererName)
	{
		for (auto& renderStage : this->RenderStageList)
		{
			renderStage->Link(rendererName);
		}
	}

	const bool& GRenderTechnique::GetActive() const noexcept
	{
		return this->IsActive;
	}
}
