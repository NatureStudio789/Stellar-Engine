#ifndef _SE_ROOTSIGNATURE_H_
#define _SE_ROOTSIGNATURE_H_
#include "../ContextDependent/ContextDependent.h"
#include "../../Core/MessageHandler/MessageHandler.h"
#include "RootParameter/RootParameter.h"
#include "StaticSamplerDescription/StaticSamplerDescription.h"

namespace SE
{
	class GRootRignature : public GContextDependent
	{
	public:
		GRootRignature();
		GRootRignature(const GRootRignature& other);
		~GRootRignature();

		unsigned int AddParameter(const GRootParameter& parameter);
		unsigned int AddSamplerDescription(const GStaticSamplerDescription& description);
		void Initialize();

		WRL::ComPtr<ID3D12RootSignature> GetInstance();

		unsigned int GetRootParameterIndex(const GRootParameter& parameter) const;
		unsigned int GetSamplerDescriptionIndex(const GStaticSamplerDescription& description) const;

		const bool& GetInitialized() const noexcept;

	private:
		WRL::ComPtr<ID3D12RootSignature> RootSignatureInstance;
		bool IsInitialized;

		std::vector<GRootParameter> ParameterList;
		std::vector<GStaticSamplerDescription> SamplerDescriptionList;
	};
}

#endif