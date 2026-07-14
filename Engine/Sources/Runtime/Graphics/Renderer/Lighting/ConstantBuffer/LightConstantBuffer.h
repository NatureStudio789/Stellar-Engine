#ifndef _SE_LIGHTCONSTANTBUFFER_H_
#define _SE_LIGHTCONSTANTBUFFER_H_
#include "../../../Buffer/ConstantBuffer.h"

#include "../PointLight/PointLight.h"
#include "../DirectionalLight/DirectionalLight.h"

namespace SE
{
	struct GPointLightCBufferData
	{
	public:
		GPointLightCBufferData()
		{
			this->PointLightCount = 0;
		}

		GPointLight::Data PointLightList[50];
		int PointLightCount;
	};

	class GPointLightCBuffer : public GConstantBuffer<GPointLightCBufferData>
	{
	public:
		using GConstantBuffer<GPointLightCBufferData>::GConstantBuffer;
	};

	struct GDirectionalLightCBufferData
	{
	public:
		GDirectionalLightCBufferData()
		{
			this->DirectionalLightCount = 0;
		}

		GDirectionalLight::Data DirectionalLightList[50];
		int DirectionalLightCount;
	};

	class GDirectionalLightCBuffer : public GConstantBuffer<GDirectionalLightCBufferData>
	{
	public:
		using GConstantBuffer<GDirectionalLightCBufferData>::GConstantBuffer;
	};
}

#endif