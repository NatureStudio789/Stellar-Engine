#ifndef _SE_CAMERA_H_
#define _SE_CAMERA_H_
#include "../../Core/Registry/Registry.h"

namespace SE
{
	template<typename T>
	class GConstantBuffer;

	class GCamera : public SAddressable
	{
	public:
		GCamera();
		GCamera(const glm::vec3& position, const glm::quat& rotation,
			float fov, float aspect, float nearZ, float farZ, bool isFreeLook);
		GCamera(const GCamera& other);
		~GCamera();

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);
		void Translate(const glm::vec3& translation);
		void Translate(float x, float y, float z);

		void SetRotation(const glm::quat& rotation);
		void SetRotation(const glm::vec3& eulerRotation);
		void Rotate(const glm::quat& rotation);
		void Rotate(const glm::vec3& eulerRotation);

		void SetFOV(float fov);
		void SetAspect(float aspect);
		void SetAspect(float width, float height);
		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);

		glm::mat4x4 GetProjectionMatrix() const noexcept;
		glm::mat4x4 GetViewMatrix() const noexcept;

		glm::vec3 Position;
		glm::quat Rotation;

		float FOV;
		float Aspect;
		float NearZ;
		float FarZ;

		bool IsFreeLook;

	private:
		void ApplyToPipeline();

		struct CBufferData
		{
			CBufferData()
			{
				this->CameraMatrix = glm::mat4x4(1.0f);
			}
			CBufferData(const glm::mat4x4& camera, const glm::vec3& position, float farZ)
			{
				this->CameraMatrix = camera;
				this->CameraPosition = position;
				this->FarZ = farZ;
			}

			glm::mat4x4 CameraMatrix;
			glm::vec3 CameraPosition;
			float FarZ;
		};

		std::shared_ptr<GConstantBuffer<CBufferData>> CameraCBuffer;

		friend class SCameraRegistry;
	};

	class SCameraRegistry : public SRegistry<GCamera>
	{
	public:
		static void SetCurrentInstance(const std::string& name);

		static void ApplyInstance(const std::string& name);
		static void ApplyInstance(const SUUID& id);
		static void ApplyCurrentInstance();

		static std::shared_ptr<GCamera> GetCurrentInstance();

	private:
		static std::string CurrentInstanceName;
	};
}

#endif