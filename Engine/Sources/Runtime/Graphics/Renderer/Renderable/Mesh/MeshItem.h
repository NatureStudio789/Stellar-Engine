#ifndef _SE_MESHITEM_H_
#define _SE_MESHITEM_H_
#include "../../RenderTechnique/RenderTechnique.h"
#include "../../../Buffer/ConstantBuffer.h"
#include "../Renderable.h"

namespace SE
{
	class GMeshItem : public GRenderable
	{
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec2 TextureCoord;
			glm::vec3 Normal;
		};

		struct Data
		{
			Data() = default;
			Data(const Data& other)
			{
				this->Name = other.Name;
				this->Vertices = other.Vertices;
				this->Indices = other.Indices;
				this->MaterialId = other.MaterialId;
			}

			std::string Name;
			std::vector<Vertex> Vertices;
			std::vector<unsigned int> Indices;
			SUUID MaterialId;
		};

	public:
		GMeshItem();
		GMeshItem(const Data& data);
		~GMeshItem() override;

		void Initialize(const Data& data);

		const Data& GetData() const noexcept;

	private:
		Data ItemData;
	};
}

#endif