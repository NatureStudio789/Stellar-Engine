#include <Core.h>
#include "Transform.h"

namespace SE
{
	STransform::STransform()
	{
		this->Position = {};
		this->Rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		this->Scale = { 1.0f, 1.0f, 1.0f };
	}

	STransform::STransform(const glm::vec3& position, glm::quat& rotation, glm::vec3& scale)
	{
		this->Position = position;
		this->Rotation = rotation;
		this->Scale = scale;
	}

	STransform::STransform(const glm::vec3& position, glm::vec3& eulerAngles, glm::vec3& scale)
	{
		this->Position = position;
		this->Rotation = glm::quat(eulerAngles);
		this->Scale = scale;
	}

	STransform::STransform(const STransform& other)
	{
		this->Position = other.Position;
		this->Rotation = other.Rotation;
		this->Scale = other.Scale;
	}

	STransform::~STransform()
	{
		this->Position = {};
		this->Rotation = {};
		this->Scale = {};
	}

	void STransform::SetPosition(const glm::vec3& position)
	{
		this->Position = position;
	}

	void STransform::SetPosition(float x, float y, float z)
	{
		this->Position = { x, y, z };
	}

	void STransform::Translate(const glm::vec3 & translation)
	{
		this->Position += translation;
	}

	void STransform::Translate(float x, float y, float z)
	{
		this->Position += glm::vec3(x, y, z);
	}

	void STransform::SetRotation(const glm::quat & rotation)
	{
		this->Rotation = rotation;
	}

	void STransform::SetRotation(const glm::vec3 & eulerRotation)
	{
		this->Rotation = glm::quat(glm::radians(eulerRotation));
	}

	void STransform::Rotate(const glm::vec3 & eulerRotation)
	{
		this->Rotation *= glm::quat(glm::radians(eulerRotation));
	}

	void STransform::Rotate(float x, float y, float z)
	{
		this->Rotation *= glm::quat(glm::radians(glm::vec3(x, y, z)));
	}

	void STransform::SetScale(const glm::vec3 & scale)
	{
		this->Scale = scale;
	}

	void STransform::SetScale(float x, float y, float z)
	{
		this->Scale = { x, y, z };
	}

	void STransform::Scaling(const glm::vec3 & scaling)
	{
		this->Scale += scaling;
	}

	void STransform::Scaling(float x, float y, float z)
	{
		this->Scale += glm::vec3(x, y, z);
	}

	glm::mat4x4 STransform::GetMatrix() const noexcept
	{
		glm::mat4x4 TranslationMatrix = { 1.0f };
		TranslationMatrix = glm::translate(TranslationMatrix, this->Position);
		glm::mat4x4 RotationMatrix = glm::mat4_cast(this->Rotation);
		glm::mat4x4 ScaleMatrix = { 1.0f };
		ScaleMatrix = glm::scale(ScaleMatrix, this->Scale);

		return TranslationMatrix * RotationMatrix * ScaleMatrix;
	}
}
