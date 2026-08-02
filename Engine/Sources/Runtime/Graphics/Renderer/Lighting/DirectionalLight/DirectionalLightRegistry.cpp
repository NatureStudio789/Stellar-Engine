#include <Core.h>
#include "DirectionalLightRegistry.h"

namespace SE
{
	GDirectionalLightRegistry::GDirectionalLightRegistry() : SInstantiableRegistry<GDirectionalLight>()
	{
		this->LightCBuffer = std::make_shared<GDirectionalLightCBuffer>(GRenderGroup::COMPOSITION_GROUP, 4);
	}

	GDirectionalLightRegistry::GDirectionalLightRegistry(const std::string& name)
	{
		this->Initialize(name);
	}

	GDirectionalLightRegistry::GDirectionalLightRegistry(const GDirectionalLightRegistry& other) : SInstantiableRegistry<GDirectionalLight>(other)
	{
		this->LightCBuffer = other.LightCBuffer;
	}

	GDirectionalLightRegistry::~GDirectionalLightRegistry()
	{

	}

	void GDirectionalLightRegistry::Initialize(const std::string& name)
	{
		this->RegistryName = name;

		this->LightCBuffer = std::make_shared<GDirectionalLightCBuffer>(GRenderGroup::COMPOSITION_GROUP, 4);

		this->DepthMapSRVSet = std::make_shared<GSRVSet>(name + "_DepthMapSet", 50);
		SSRVSetRegistry::Register(this->DepthMapSRVSet);
		this->DepthMapSRVSet->SetRootParameterIndex(SPipelineStateRegistry::GetInstance(GRenderGroup::COMPOSITION_GROUP)->
			GetRootSignature()->GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, 5, 50)));
	}

	void GDirectionalLightRegistry::Register(std::shared_ptr<GDirectionalLight> instance)
	{
		SInstantiableRegistry::Register(instance);

		auto DepthMap = std::make_shared<GDepthMap>(glm::uvec2{ 1024, 1024 });
		DepthMap->SetName(instance->GetName() + "_DepthMap");
		this->DepthMapList.push_back(DepthMap);

		this->LightCameraMatrixList.push_back(glm::mat4x4(1.0f));

		auto LightCameraCBuffer = std::make_shared<GConstantBuffer<glm::mat4x4>>(GRenderGroup::SHADOW_GROUP, 1);
		this->LightCameraCBufferList.push_back(LightCameraCBuffer);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(std::shared_ptr<GDirectionalLight> instance)
	{
		SInstantiableRegistry::Unregister(instance);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(std::string name)
	{
		SInstantiableRegistry::Unregister(name);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Unregister(SUUID uuid)
	{
		SInstantiableRegistry::Unregister(uuid);

		this->UpdateLightData();
	}

	void GDirectionalLightRegistry::Update()
	{
		unsigned int index = 0;
		for (auto [uuid, light] : this->RegisteredInstanceList)
		{
			auto DepthMap = this->DepthMapList[index];
			if (DepthMap->GetName() == (light->GetName() + "_DepthMap"))
			{
				this->DepthMapSRVSet->AppendSRV(DepthMap->GetShaderResourceView().get());
				DepthMap->RebindSRV();
			}

			index++;
		}

		this->DepthMapSRVSet->ResetMemberState();
	}

	void GDirectionalLightRegistry::UpdateLightData()
	{
		GDirectionalLightCBufferData Data;
		Data.DirectionalLightCount = (int)this->RegisteredInstanceList.size();

		unsigned int index = 0;
		for (auto [uuid, light] : this->RegisteredInstanceList)
		{
			Data.DirectionalLightList[index] = light->LightData;
			index++;
		}

		this->LightCBuffer->UpdateData(Data);
	}
}
