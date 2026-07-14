#ifndef _SE_DIRECTIONALLIGHT_H_
#define _SE_DIRECTIONALLIGHT_H_
#include "../../../../Core/Addressable/Addressable.h"

namespace SE
{
	class GDirectionalLight : public SAddressable
	{
	public:
		struct Data
		{
		public:
			Data()
			{
				this->Direction = {};
				this->Strength = 1000.0f;
				this->Color = { 1.0f, 1.0f, 1.0f };
			}
			Data(const glm::vec3& direction, float strength, const glm::vec3& color)
			{
				this->Direction = direction;
				this->Strength = strength;
				this->Color = color;
			}
			Data(const Data& other)
			{
				this->Direction = other.Direction;
				this->Strength = other.Strength;
				this->Color = other.Color;
			}

		public:
			glm::vec3 Direction;
			float Strength;
			glm::vec3 Color;

		private:
			float padding;
		};

	public:
		GDirectionalLight();
		GDirectionalLight(const std::string& name, const Data& data);
		GDirectionalLight(const GDirectionalLight& other);
		~GDirectionalLight() override;

	public:
		Data LightData;
	};
}

#endif