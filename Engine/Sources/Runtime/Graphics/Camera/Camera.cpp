#include <Core.h>
#include "../Buffer/ConstantBuffer.h"
#include "Camera.h"

namespace SE
{
	GCamera::GCamera() : SAddressable()
	{
		this->Position = {};
		this->Rotation = glm::quat(glm::vec3(0.0f));

		this->FOV = 60.0f;
		this->Aspect = 16.0f / 9.0f;
		this->NearZ = 0.1f;
		this->FarZ = 1000.0f;

		this->IsFreeLook = false;

		this->CameraCBuffer = std::make_shared<GConstantBuffer<CBufferData>>(GRenderGroup::ALBEDO_GROUP, 2);

		this->Activate();
	}

	GCamera::GCamera(const glm::vec3& position, const glm::quat& rotation, 
		float fov, float aspect, float nearZ, float farZ, bool isFreeLook) : SAddressable()
	{
		this->Position = position;
		this->Rotation = rotation;
		
		this->FOV = fov;
		this->Aspect = aspect;
		this->NearZ = nearZ;
		this->FarZ = farZ;

		this->IsFreeLook = isFreeLook;

		this->CameraCBuffer = std::make_shared<GConstantBuffer<CBufferData>>(GRenderGroup::ALBEDO_GROUP, 2);

		this->Activate();
	}

	GCamera::GCamera(const GCamera & other) : SAddressable(other)
	{
		this->Position = other.Position;
		this->Rotation = other.Rotation;

		this->FOV = other.FOV;
		this->Aspect = other.Aspect;
		this->NearZ = other.NearZ;
		this->FarZ = other.FarZ;
		
		this->IsFreeLook = other.IsFreeLook;

		this->CameraCBuffer = other.CameraCBuffer;
	}

	GCamera::~GCamera()
	{

	}

	void GCamera::SetPosition(const glm::vec3 & position)
	{
		this->Position = position;
	}

	void GCamera::SetPosition(float x, float y, float z)
	{
		this->Position = { x, y ,z };
	}

	void GCamera::Translate(const glm::vec3 & translation)
	{
		this->Position += translation;
	}

	void GCamera::Translate(float x, float y, float z)
	{
		this->Position += glm::vec3{ x, y, z };
	}

	void GCamera::SetRotation(const glm::quat & rotation)
	{
		this->Rotation = rotation;

		this->UpdateVectors();
	}

	void GCamera::SetRotation(const glm::vec3 & eulerRotation)
	{
		this->Rotation = glm::quat(glm::radians(eulerRotation));

		this->UpdateVectors();
	}

	void GCamera::Rotate(const glm::vec3 & eulerRotation)
	{
		glm::quat yawQuat = glm::angleAxis(glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 localRight = this->Rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		glm::quat pitchQuat = glm::angleAxis(glm::radians(eulerRotation.x), localRight);

		this->Rotation = glm::normalize(yawQuat * pitchQuat * this->Rotation);

		this->UpdateVectors();
	}

	void GCamera::SetFOV(float fov)
	{
		this->FOV = fov;
	}

	void GCamera::SetAspect(float aspect)
	{
		this->Aspect = aspect;
	}

	void GCamera::SetAspect(float width, float height)
	{
		this->Aspect = width / height;
	}

	void GCamera::SetNearZ(float nearZ)
	{
		this->NearZ = nearZ;
	}

	void GCamera::SetFarZ(float farZ)
	{
		this->FarZ = farZ;
	}

	glm::mat4x4 GCamera::GetProjectionMatrix() const noexcept
	{
		return glm::perspectiveLH_ZO(glm::radians(this->FOV), this->Aspect, this->NearZ, this->FarZ);
	}

	glm::mat4x4 GCamera::GetViewMatrix() const noexcept
	{
		glm::vec3 CameraFront = this->Rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp;
		if (this->IsFreeLook)
		{
			CameraUp = this->Rotation * glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		return glm::lookAtLH(this->Position, CameraFront + this->Position, CameraUp);
	}

	void GCamera::UpdateVectors()
	{
		glm::vec3 euler = glm::eulerAngles(this->Rotation);
		glm::quat Pitch = glm::angleAxis(euler.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat Yaw = glm::angleAxis(euler.y, glm::vec3(0.0f, 1.0f, 0.0f));

		if (this->IsFreeLook)
		{
			this->Forward = (Pitch * Yaw) * glm::vec3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			this->Forward = Yaw * glm::vec3(0.0f, 0.0f, -1.0f);
		}

		this->Right = Yaw * glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	void GCamera::ApplyToPipeline()
	{
		glm::mat4x4 CameraMatrix = this->GetProjectionMatrix() * this->GetViewMatrix();
		CameraMatrix = glm::transpose(CameraMatrix);
		this->CameraCBuffer->UpdateData({ CameraMatrix, this->Position, this->FarZ });

		this->CameraCBuffer->Apply();
	}


	std::string SCameraRegistry::CurrentInstanceName;

	void SCameraRegistry::SetCurrentInstance(const std::string& name)
	{
		CurrentInstanceName = name;
	}

	void SCameraRegistry::ApplyInstance(const std::string& name)
	{
		GetInstance(name)->ApplyToPipeline();
	}

	void SCameraRegistry::ApplyInstance(const SUUID& id)
	{
		GetInstance(id)->ApplyToPipeline();
	}
	
	void SCameraRegistry::ApplyCurrentInstance()
	{
		ApplyInstance(CurrentInstanceName);
	}

	std::shared_ptr<GCamera> SCameraRegistry::GetCurrentInstance()
	{
		return GetInstance(CurrentInstanceName);
	}
}
