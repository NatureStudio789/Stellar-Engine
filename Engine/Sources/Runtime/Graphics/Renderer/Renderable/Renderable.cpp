#include <Core.h>
#include "Renderable.h"

namespace SE
{
	GRenderable::GRenderable() : SAddressable()
	{
		this->VertexBuffer = null;
		this->IndexBuffer = null;
		this->Topology = null;

		this->RenderTechniqueList.clear();
	}

	GRenderable::GRenderable(const std::string& name, std::shared_ptr<GVertexBuffer> vertexBuffer, 
		std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology) : SAddressable()
	{
		this->Initialize(name, vertexBuffer, indexBuffer, topology);
	}

	GRenderable::GRenderable(const GRenderable & other) : SAddressable(other)
	{
		this->VertexBuffer = other.VertexBuffer;
		this->IndexBuffer = other.IndexBuffer;
		this->Topology = other.Topology;

		this->RenderTechniqueList = other.RenderTechniqueList;
	}

	GRenderable::~GRenderable()
	{
		this->VertexBuffer.reset();
		this->IndexBuffer.reset();
		this->Topology.reset();

		for (auto& technique : this->RenderTechniqueList)
		{
			technique.reset();
		}
		this->RenderTechniqueList.clear();
	}

	void GRenderable::Initialize(const std::string & name, std::shared_ptr<GVertexBuffer> vertexBuffer, 
		std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology)
	{
		this->SetName(name);

		this->VertexBuffer = vertexBuffer;
		this->IndexBuffer = indexBuffer;
		this->Topology = topology;

		this->Activate();
	}

	void GRenderable::AddRenderTechnique(std::shared_ptr<GRenderTechnique> technique)
	{
		this->RenderTechniqueList.push_back(technique);
	}

	void GRenderable::SetRenderTechniqueList(std::vector<std::shared_ptr<GRenderTechnique>> techniqueList)
	{
		this->RenderTechniqueList = techniqueList;
	}

	void GRenderable::Apply()
	{
		this->VertexBuffer->Apply();

		if (this->HasIndexBuffer())
		{
			this->IndexBuffer->Apply();
		}

		this->Topology->Apply();
	}

	void GRenderable::Submit(const std::string& channel)
	{
		for (auto& technique : this->RenderTechniqueList)
		{
			technique->Submit(std::make_shared<GRenderable>(*this), channel);
		}
	}

	void GRenderable::LinkTechnique(const std::string & rendererName)
	{
		for (auto& technique : this->RenderTechniqueList)
		{
			technique->Link(rendererName);
		}
	}

	const unsigned int& GRenderable::GetVerticesCount() const noexcept
	{
		return this->VertexBuffer->GetVerticesCount();
	}

	unsigned int GRenderable::GetIndicesCount() const noexcept
	{
		if (!this->HasIndexBuffer())
		{
			return 0;
		}
		else
		{
			return this->IndexBuffer->GetIndicesCount();
		}
	}

	const unsigned int& GRenderable::GetBufferDataSize() const noexcept
	{
		if (this->HasIndexBuffer())
		{
			return this->IndexBuffer->GetIndicesCount();
		}
		else
		{
			return this->VertexBuffer->GetVerticesCount();
		}
	}

	bool GRenderable::HasIndexBuffer() const noexcept
	{
		return this->IndexBuffer != null;
	}
}
