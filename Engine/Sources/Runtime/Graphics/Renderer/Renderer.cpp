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
		// TO DO: Make the resource link from global to passes.

		std::vector<ID3D12GraphicsCommandList*> GraphicsCommandLists;

		for (auto& renderPass : this->RenderPassList)
		{
			renderPass->Execute();

			GraphicsCommandLists.push_back(
				renderPass->PassCommandList->GetInstance().Get());

			// TO DO: Make the resource link from pass to other passes.
		}

		this->GetContext()->ExecuteCommandLists(GraphicsCommandLists);
	}
}
