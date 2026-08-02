#include <Core.h>

#include "../../Renderable/Renderable.h"
#include "../../Lighting/DirectionalLight/DirectionalLightRegistry.h"

#include "ShadowPass.h"

namespace SE
{
	static glm::mat4x4 ComputeLightCameraMatrix(const glm::vec3& cameraPosition,
		const glm::vec3& forward, const glm::vec3& right, const glm::vec3& up,
		float fovY, float aspect, float nearZ, float farZ, glm::vec3 lightDirection, float zPadding);

	GShadowPass::GShadowPass() : GRenderQueuePass()
	{

	}

	GShadowPass::GShadowPass(const std::string& name) : GRenderQueuePass()
	{
		this->Initialize(name);
	}

	GShadowPass::GShadowPass(const GShadowPass & other) : GRenderQueuePass(other)
	{
		this->DirectionalLightRegistry = other.DirectionalLightRegistry;
	}

	GShadowPass::~GShadowPass()
	{
		
	}

	void GShadowPass::Initialize(const std::string & name)
	{
		this->RenderPassName = name;

		this->EnableUniversalCameraForRendering = false;

		this->DirectionalDepthMapListOutflow = std::make_shared<GOutflow>("DirectionalDepthMapSRVList", GResourcePackage{}, true);
		this->AddOutflow(this->DirectionalDepthMapListOutflow);

		this->AddApplicable(SPipelineStateRegistry::GetInstance(GRenderGroup::SHADOW_GROUP));
	}

	void GShadowPass::SetLightRegistry(std::shared_ptr<GDirectionalLightRegistry> registry)
	{
		this->DirectionalLightRegistry = registry;

		this->DirectionalDepthMapListOutflow->SetResourcePackage(this->DirectionalLightRegistry->DepthMapSRVSet->GetResourcePackage());
	}

	void GShadowPass::Apply()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}
	}

	void GShadowPass::Execute()
	{
		{
			this->DirectionalLightRegistry->Update();

			GDirectionalLightCBufferData LightCBufferData;
			unsigned int index = 0;
			for (auto [uuid, directional] : this->DirectionalLightRegistry->RegisteredInstanceList)
			{
				auto& CameraMatrix = this->DirectionalLightRegistry->LightCameraMatrixList[index];

				//Calculate Light Camera Matrix
				auto MainCamera = SCameraRegistry::GetInstance(this->RenderAttribution->MainCameraId);
				CameraMatrix = ComputeLightCameraMatrix(MainCamera->Position, MainCamera->Forward,
					MainCamera->Right, MainCamera->Up, glm::radians(MainCamera->FOV), MainCamera->Aspect, MainCamera->NearZ,
					MainCamera->FarZ, directional->LightData.Direction, 0.0f);

				glm::mat4x4 NDCToTexSpace = 
				{
					0.5f, 0.0f, 0.0f, 0.0f,
					0.0f, -0.5f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.5f, 0.5f, 0.0f, 1.0f
				};
				glm::mat4x4 TransCameraMatrix = NDCToTexSpace * CameraMatrix;

				LightCBufferData.LightCameraMatrixList[index] = glm::transpose(TransCameraMatrix);
				LightCBufferData.DirectionalLightCount = (UINT)this->DirectionalLightRegistry->RegisteredInstanceList.size();
				LightCBufferData.DirectionalLightList[index] = directional->LightData;
				auto LightCameraCBuffer = this->DirectionalLightRegistry->LightCameraCBufferList[index];
				LightCameraCBuffer->UpdateData(glm::transpose(CameraMatrix));

				auto DepthMap = this->DirectionalLightRegistry->DepthMapList[index];
				DepthMap->Begin();

				this->GetContext()->ApplyDescriptorHeaps();
				this->Apply();
				DepthMap->Clear();
				DepthMap->Apply();
				LightCameraCBuffer->Apply();

				for (auto drawTask : this->TaskList)
				{
					drawTask->Execute();
				}

				DepthMap->End();

				index++;
			}

			this->DirectionalLightRegistry->LightCBuffer->UpdateData(LightCBufferData);
		}

		this->TaskList.clear();
	}

	glm::mat4x4 ComputeLightCameraMatrix(const glm::vec3& cameraPosition,
		const glm::vec3& forward, const glm::vec3& right, const glm::vec3& up,
		float fovY, float aspect, float nearZ, float farZ, glm::vec3 lightDirection, float zPadding)
	{
		glm::vec3 NearCenter = cameraPosition + forward * nearZ;
		glm::vec3 FarCenter = cameraPosition + forward * farZ;

		float NearHalfH = nearZ * tanf(fovY * 0.5f);
		float NearHalfW = NearHalfH * aspect;
		float FarHalfH = farZ * tanf(fovY * 0.5f);
		float FarHalfW = FarHalfH * aspect;

		glm::vec3 Corners[8];
		Corners[0] = NearCenter + right * NearHalfW + up * NearHalfH;
		Corners[1] = NearCenter - right * NearHalfW + up * NearHalfH;
		Corners[2] = NearCenter - right * NearHalfW - up * NearHalfH;
		Corners[3] = NearCenter + right * NearHalfW - up * NearHalfH;
		Corners[4] = FarCenter + right * FarHalfW + up * FarHalfH;
		Corners[5] = FarCenter - right * FarHalfW + up * FarHalfH;
		Corners[6] = FarCenter - right * FarHalfW - up * FarHalfH;
		Corners[7] = FarCenter + right * FarHalfW - up * FarHalfH;

		glm::vec3 FrustumCenter(0.0f);
		for (int i = 0; i < 8; ++i) FrustumCenter += Corners[i];
		FrustumCenter /= 8.0f;

		float Dist = glm::length(Corners[0] - Corners[7]) * 2.0f;
		glm::vec3 LightPos = FrustumCenter - lightDirection * Dist;

		glm::vec3 Up = glm::vec3(0, 1, 0);
		if (glm::abs(glm::dot(Up, lightDirection)) > 0.999f) Up = glm::vec3(1, 0, 0);
		glm::mat4 LightView = glm::lookAtLH(LightPos, FrustumCenter, Up);

		glm::vec3 MinP(FLT_MAX), MaxP(-FLT_MAX);
		for (int i = 0; i < 8; ++i) 
		{
			glm::vec3 Ls = glm::vec3(LightView * glm::vec4(Corners[i], 1.0f));
			MinP = glm::min(MinP, Ls);
			MaxP = glm::max(MaxP, Ls);
		}
		MinP.z -= zPadding;
		MaxP.z += zPadding;

		glm::mat4 LightProj = glm::orthoLH_ZO(MinP.x, MaxP.x, MinP.y, MaxP.y, MinP.z, MaxP.z);
		return LightProj * LightView;
	}
}
