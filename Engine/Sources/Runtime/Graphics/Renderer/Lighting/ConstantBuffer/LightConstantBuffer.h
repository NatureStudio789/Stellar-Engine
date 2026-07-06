#ifndef _SE_LIGHTCONSTANTBUFFER_H_
#define _SE_LIGHTCONSTANTBUFFER_H_
#include "../../../Buffer/ConstantBuffer.h"

#include "../PointLight/PointLight.h"

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
}

#endif