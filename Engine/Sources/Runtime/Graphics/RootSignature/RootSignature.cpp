#include <Core.h>
#include "RootSignature.h"

namespace SE
{
	GRootSignature::GRootSignature() : GContextDependent()
	{
		this->IsInitialized = false;

		this->ParameterList.clear();
		this->SamplerDescriptionList.clear();
	}

	GRootSignature::GRootSignature(const GRootSignature& other) : GContextDependent(other)
	{
		this->RootSignatureInstance = other.RootSignatureInstance;
		this->IsInitialized = other.IsInitialized;

		this->ParameterList = other.ParameterList;
		this->SamplerDescriptionList = other.SamplerDescriptionList;
	}

	GRootSignature::~GRootSignature()
	{
		this->IsInitialized = false;

		this->ParameterList.clear();
		this->SamplerDescriptionList.clear();
	}

	unsigned int GRootSignature::AddParameter(const GRootParameter& parameter)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Cannot add new parameter into an initialized root signature!");
		}

		unsigned int CurrentIndex = (unsigned int)this->ParameterList.size();
		this->ParameterList.push_back(parameter);

		return CurrentIndex;
	}

	unsigned int GRootSignature::AddSamplerDescription(const GStaticSamplerDescription& description)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "Cannot add new sampler description into an initialized root signature!");
		}

		unsigned int CurrentIndex = (unsigned int)this->SamplerDescriptionList.size();
		this->SamplerDescriptionList.push_back(description);

		return CurrentIndex;
	}

	void GRootSignature::Initialize()
	{
		std::vector<CD3DX12_ROOT_PARAMETER> D3DParameterList;
		for (const auto& parameter : this->ParameterList)
		{
			CD3DX12_ROOT_PARAMETER Parameter;

			switch (Parameter.ParameterType)
			{
				case GRootParameter::SE_PARAMETER_CBV:
				{
					Parameter.InitAsConstantBufferView(parameter.ShaderRegisterIndex);

					break;
				}

				case GRootParameter::SE_PARAMETER_SRV:
				{
					CD3DX12_DESCRIPTOR_RANGE ShaderResourceViewTable;
					ShaderResourceViewTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, parameter.DescriptorCount, parameter.ShaderRegisterIndex);

					Parameter.InitAsDescriptorTable(1, &ShaderResourceViewTable);
					break;
				}
			}

			D3DParameterList.push_back(Parameter);
		}

		std::vector<CD3DX12_STATIC_SAMPLER_DESC> DescriptionList;
		for (auto& description : this->SamplerDescriptionList)
		{
			CD3DX12_STATIC_SAMPLER_DESC Description;

			Description = CD3DX12_STATIC_SAMPLER_DESC(description.ShaderRegister, description.Filter,
				description.AddressU, description.AddressV, description.AddressW);

			DescriptionList.push_back(Description);
		}

		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
			(unsigned int)D3DParameterList.size(), D3DParameterList.data(),
			(unsigned int)DescriptionList.size(), DescriptionList.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		WRL::ComPtr<ID3DBlob> SerializedRootSignature;
		WRL::ComPtr<ID3DBlob> ErrorBuffer;
		HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSignature.GetAddressOf(), ErrorBuffer.GetAddressOf());
		if (ErrorBuffer)
		{
#ifdef SE_DEBUG
			::OutputDebugStringA((char*)ErrorBuffer->GetBufferPointer());
#endif
		}
		SMessageHandler::Instance->Check(hr);

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateRootSignature(0,
			SerializedRootSignature->GetBufferPointer(), SerializedRootSignature->GetBufferSize(),
			__uuidof(ID3D12RootSignature), (void**)this->RootSignatureInstance.GetAddressOf()));

		this->IsInitialized = true;
	}

	WRL::ComPtr<ID3D12RootSignature> GRootSignature::GetInstance()
	{
		return this->RootSignatureInstance;
	}

	unsigned int GRootSignature::GetRootParameterIndex(const GRootParameter& parameter) const
	{
		int Index = -1;
		for (UINT i = 0; i < (UINT)this->ParameterList.size(); i++)
		{
			if (this->ParameterList[i] == parameter)
			{
				Index = i;
				break;
			}
		}

		if (Index < 0)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "No proper parameter found in root signature!");
			return 0;
		}

		return (unsigned int)Index;
	}

	unsigned int GRootSignature::GetSamplerDescriptionIndex(const GStaticSamplerDescription& description) const
	{
		int Index = -1;
		for (UINT i = 0; i < (UINT)this->SamplerDescriptionList.size(); i++)
		{
			if (this->SamplerDescriptionList[i] == description)
			{
				Index = i;
				break;
			}
		}

		if (Index < 0)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "No proper sampler description found in root signature!");
			return 0;
		}

		return (unsigned int)Index;
	}

	const bool& GRootSignature::GetInitialized() const noexcept
	{
		return this->IsInitialized;
	}
}
