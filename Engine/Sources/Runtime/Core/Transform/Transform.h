#ifndef _SE_TRANSFORM_H_
#define _SE_TRANSFORM_H_

namespace SE
{
	class STransform
	{
	public:
		STransform();
		STransform(const glm::vec3& position, glm::quat& rotation, glm::vec3& scale);
		STransform(const glm::vec3& position, glm::vec3& eulerAngles, glm::vec3& scale);
		STransform(const STransform& other);
		~STransform();

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);
		void Translate(const glm::vec3& translation);
		void Translate(float x, float y, float z);
		
		void SetRotation(const glm::quat& rotation);
		void SetRotation(const glm::vec3& eulerRotation);
		void Rotate(const glm::vec3& eulerRotation);
		void Rotate(float x, float y, float z);

		void SetScale(const glm::vec3& scale);
		void SetScale(float x, float y, float z);
		void Scaling(const glm::vec3& scaling);
		void Scaling(float x, float y, float z);

		glm::mat4x4 GetMatrix() const noexcept;

		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;
	};
}

#endif	