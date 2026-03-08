#include <Core.h>
#include "../PipelineState/PipelineState.h"
#include "../CommandList/CommandList.h"
#include "../Context/GraphicsContext.h"
#include "Texture.h"

namespace SE
{
	GTexture::GTexture() : SAddressable(), GShaderResourceView()
	{
		this->BelongRenderGoup = "";
	}

	GTexture::GTexture(const std::string& filePath, 
		unsigned int shaderRegisterIndex, const std::string& renderGroup) : SAddressable(), GShaderResourceView()
	{
		this->Initialize(filePath, shaderRegisterIndex, renderGroup);
	}

	GTexture::GTexture(const GTexture& other) : SAddressable(other), GShaderResourceView(other)
	{
		this->BelongRenderGoup = other.BelongRenderGoup;

		this->TextureBuffer = other.TextureBuffer;
		this->UploadBuffer = other.UploadBuffer;
	}

	GTexture::~GTexture()
	{
		this->BelongRenderGoup.clear();
	}

	void GTexture::Initialize(const std::string& filePath, unsigned int shaderRegisterIndex, const std::string& renderGroup)
	{
		if (GRenderGroup::CheckAvailable(renderGroup))
		{
			this->BelongRenderGoup = renderGroup;

			this->RootParameterIndex = SPipelineStateRegistry::GetInstance(this->BelongRenderGoup)->
				GetRootSignature()->GetRootParameterIndex(GRootParameter(GRootParameter::SE_PARAMETER_SRV, shaderRegisterIndex));
		}
		else
		{
			this->RootParameterIndex = 0;
		}

		this->AllocateDescriptor();

		GImage TextureImage(filePath);

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureImage.GetWidth(), TextureImage.GetHeight(), 1, 1),
			D3D12_RESOURCE_STATE_COPY_DEST, null, __uuidof(ID3D12Resource), (void**)this->TextureBuffer.GetAddressOf()));

		const auto UploadBufferSize = GetRequiredIntermediateSize(this->TextureBuffer.Get(), 0, 1);
		SMessageHandler::Instance->Check(this->GetDeviceInstance()->
			CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));

		D3D12_SUBRESOURCE_DATA TextureSubresourceData;
		STELLAR_CLEAR_MEMORY(TextureSubresourceData);
		TextureSubresourceData.pData = (const void*)TextureImage.GetData();
		TextureSubresourceData.RowPitch = (long long)(((TextureImage.GetWidth() * sizeof(GColor)) + 3) & ~3);
		TextureSubresourceData.SlicePitch = (long long)TextureSubresourceData.RowPitch * TextureImage.GetHeight();
		UpdateSubresources(this->GetInitializationCommandListInstance().Get(),
			this->TextureBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &TextureSubresourceData);

		this->GetInitializationCommandListInstance()->
			ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(this->TextureBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		this->GetDeviceInstance()->
			CreateShaderResourceView(this->TextureBuffer.Get(), null, this->DescriptorHandle->CPUHandle);

		this->GetContext()->ExecuteInitialization();

		this->Activate();
	}

	void GTexture::Apply()
	{
		if (!GRenderGroup::CheckAvailable(this->BelongRenderGoup))
		{
			SMessageHandler::Instance->SetFatal("Graphics", "This texture is NOT available for applying!");
		}

		GShaderResourceView::Apply();
	}
}
