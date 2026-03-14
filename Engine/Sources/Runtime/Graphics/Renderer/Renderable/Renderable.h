#ifndef _SE_RENDERABLE_H_
#define _SE_RENDERABLE_H_
#include "../../../Core/Addressable/Addressable.h"

#include "../../Buffer/VertexBuffer.h"
#include "../../Buffer/IndexBuffer.h"
#include "../../Topology/Topology.h"
#include "../RenderTechnique/RenderTechnique.h"

namespace SE
{
	class GRenderable : public SAddressable
	{
	public:
		GRenderable();
		GRenderable(const std::string& name, std::shared_ptr<GVertexBuffer> vertexBuffer,
			std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology);
		GRenderable(const GRenderable& other);
		virtual ~GRenderable();

		void Initialize(const std::string& name, std::shared_ptr<GVertexBuffer> vertexBuffer,
			std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology);

		void AddRenderTechnique(std::shared_ptr<GRenderTechnique> technique);
		void SetRenderTechniqueList(std::vector<std::shared_ptr<GRenderTechnique>> techniqueList);

		void Apply();
		void Submit(const std::string& channel);

		void LinkTechnique(const std::string& rendererName);

		const unsigned int& GetVerticesCount() const noexcept;
		unsigned int GetIndicesCount() const noexcept;
		const unsigned int& GetBufferDataSize() const noexcept;
		bool HasIndexBuffer() const noexcept;

	private:
		std::shared_ptr<GVertexBuffer> VertexBuffer;
		std::shared_ptr<GIndexBuffer> IndexBuffer;
		std::shared_ptr<GTopology> Topology;

		std::vector<std::shared_ptr<GRenderTechnique>> RenderTechniqueList;
	};
}

#endif