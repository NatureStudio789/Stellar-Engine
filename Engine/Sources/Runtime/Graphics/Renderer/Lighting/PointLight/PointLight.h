#ifndef _SE_POINTLIGHT_H_
#define _SE_POINTLIGHT_H_
#include "../../../../Core/Addressable/Addressable.h"

namespace SE
{
	class GPointLight : public SAddressable
	{
	public:
		struct Data
		{
		public:
			Data()
			{
				this->Position = {};
				this->Strength = 1000.0f;
				this->Color = { 1.0f, 1.0f, 1.0f };
			}

			Data(const glm::vec3& position, float strength, const glm::vec3& color)
			{
				this->Position = position;
				this->Strength = strength;
				this->Color = color;
			}

			Data(const Data& other)
			{
				this->Position = other.Position;
				this->Strength = other.Strength;
				this->Color = other.Color;
			}

		public:
			glm::vec3 Position;
			float Strength;
			glm::vec3 Color;

		private:
			float padding;
		};

	public:
		GPointLight();
		GPointLight(const std::string& name, const Data& data);
		GPointLight(const GPointLight& other);
		~GPointLight() override;

	public:
		Data LightData;
	};
}

#endif