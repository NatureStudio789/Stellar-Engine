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
				this->Vertices = other.Vertices;
				this->Indices = other.Indices;
			}

			std::vector<Vertex> Vertices;
			std::vector<unsigned int> Indices;
		};

	public:
		GMeshItem();
		GMeshItem(const std::string& name, const Data& data);
		~GMeshItem() override;

		void Initialize(const std::string& name, const Data& data);

		const Data& GetData() const noexcept;

	private:
		Data ItemData;
	};
}

#endif