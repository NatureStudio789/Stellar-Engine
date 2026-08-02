#ifndef _SE_DIRECTIONALLIGHTREGISTRY_H_
#define _SE_DIRECTIONALLIGHTREGISTRY_H_
#include "../../../../Core/Registry/InstantiableRegistry.h"

#include "DirectionalLight.h"
#include "../ConstantBuffer/LightConstantBuffer.h"
#include "../../../Buffer/ConstantBuffer.h"
#include "../../../Framebuffer/DepthMap/DepthMap.h"
#include "../../../ShaderResourceView/SRVSet.h"

namespace SE
{
	class GDirectionalLightRegistry : public SInstantiableRegistry<GDirectionalLight>
	{
	public:
		GDirectionalLightRegistry();
		GDirectionalLightRegistry(const std::string& name);
		GDirectionalLightRegistry(const GDirectionalLightRegistry& other);
		~GDirectionalLightRegistry() override;

		void Initialize(const std::string& name);

		void Register(std::shared_ptr<GDirectionalLight> instance) override;
		void Unregister(std::shared_ptr<GDirectionalLight> instance) override;
		void Unregister(std::string name) override;
		void Unregister(SUUID uuid) override;

		void Update();

	private:
		void UpdateLightData();

		std::string RegistryName;

		std::shared_ptr<GDirectionalLightCBuffer> LightCBuffer;

		std::shared_ptr<GSRVSet> DepthMapSRVSet;
		std::vector<std::shared_ptr<GDepthMap>> DepthMapList;
		std::vector<glm::mat4x4> LightCameraMatrixList;
		std::vector<std::shared_ptr<GConstantBuffer<glm::mat4x4>>> LightCameraCBufferList;

		friend class GDeferredRenderer;
		friend class GShadowPass;
	};
}

#endif