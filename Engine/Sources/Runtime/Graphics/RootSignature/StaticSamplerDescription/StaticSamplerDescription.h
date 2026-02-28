#ifndef _SE_STATICSAMPLERDESCRIPTION_H_
#define _SE_STATICSAMPLERDESCRIPTION_H_

namespace SE
{
	class GStaticSamplerDescription
	{
	public:
		GStaticSamplerDescription() = default;
		GStaticSamplerDescription(unsigned int index, D3D12_FILTER filter,
			D3D12_TEXTURE_ADDRESS_MODE addressU,
			D3D12_TEXTURE_ADDRESS_MODE addressV,
			D3D12_TEXTURE_ADDRESS_MODE addressW);
		GStaticSamplerDescription(const GStaticSamplerDescription& other);

		bool operator==(const GStaticSamplerDescription& other) const noexcept;

		unsigned int ShaderRegister;
		D3D12_FILTER Filter;
		D3D12_TEXTURE_ADDRESS_MODE AddressU;
		D3D12_TEXTURE_ADDRESS_MODE AddressV;
		D3D12_TEXTURE_ADDRESS_MODE AddressW;
	};
}

#endif