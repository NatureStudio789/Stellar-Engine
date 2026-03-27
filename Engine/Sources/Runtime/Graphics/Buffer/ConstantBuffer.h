#ifndef _SE_CONSTANTBUFFER_H_
#define _SE_CONSTANTBUFFER_H_
#include "../Applicable/Applicable.h"

#include "../PipelineState/PipelineState.h"
#include "../CommandList/CommandList.h"

namespace SE
{
	template<typename DataType>
	class GConstantBuffer : public GApplicable
	{
	public:
		GConstantBuffer() = default;
		GConstantBuffer(const std::string& renderGroupName, unsigned int shaderRegisterIndex = 0);
		GConstantBuffer(const GConstantBuffer& other);
		virtual ~GConstantBuffer() override;

		void Initialize(const std::string& renderGroupName, unsigned int shaderRegisterIndex = 0);

		void UpdateData(DataType data);
		virtual void Apply() override;

	protected:
		std::string BelongRenderGroup;

		unsigned int RootParameterIndex;

		WRL::ComPtr<ID3D12Resource> UploadBuffer;
		DataType BufferData;
		unsigned int DataSize;
		unsigned char* MappedData;
	};

	template<typename DataType>
	inline GConstantBuffer<DataType>::GConstantBuffer(const std::string& renderGroupName, unsigned int shaderRegisterIndex)
	{
		this->Initialize(renderGroupName, shaderRegisterIndex);
	}

	template<typename DataType>
	inline GConstantBuffer<DataType>::GConstantBuffer(const GConstantBuffer& other) : GApplicable(other)
	{
		this->BelongRenderGroup = other.BelongRenderGroup;

		this->RootParameterIndex = other.RootParameterIndex;

		this->UploadBuffer = other.UploadBuffer;
		this->BufferData = other.BufferData;
		this->DataSize = other.DataSize;
		this->MappedData = other.MappedData;
	}

	template<typename DataType>
	inline GConstantBuffer<DataType>::~GConstantBuffer()
	{
		this->BelongRenderGroup.clear();

		this->RootParameterIndex = 0;

		this->BufferData = {};
		this->DataSize = 0;
		this->MappedData = null;
	}

	template<typename DataType>
	inline void GConstantBuffer<DataType>::Initialize(const std::string & renderGroupName, unsigned int shaderRegisterIndex)
	{
		this->BelongRenderGroup = renderGroupName;

		this->RootParameterIndex = SPipelineStateRegistry::GetInstance(this->BelongRenderGroup)->
			GetRootSignature()->GetRootParameterIndex({ GRootParameter::SE_PARAMETER_CBV, shaderRegisterIndex });

		this->DataSize = sizeof(DataType) + (16 - (sizeof(DataType) % 16));

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(this->DataSize), D3D12_RESOURCE_STATE_GENERIC_READ, null,
			__uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		SMessageHandler::Instance->Check(this->UploadBuffer->Map(0, null, (void**)this->MappedData));
	}

	template<typename DataType>
	inline void GConstantBuffer<DataType>::UpdateData(DataType data)
	{
		this->BufferData = data;

		memcpy((void*)this->MappedData, (const void*)&this->BufferData, sizeof(DataType));
	}

	template<typename DataType>
	inline void GConstantBuffer<DataType>::Apply()
	{
		SCommandListRegistry::GetCurrentInstance()->GetInstance()->SetGraphicsRootDescriptorTable(
			this->RootParameterIndex, this->UploadBuffer->GetGPUVirtualAddress());
	}
}

#endif