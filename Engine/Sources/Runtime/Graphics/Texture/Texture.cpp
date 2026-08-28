#include <Core.h>

#include "../PipelineState/PipelineState.h"
#include "../CommandList/CommandList.h"
#include "../Context/GraphicsContext.h"

#include "../../Asset/Loader/AssetLoader.h"

#include "Texture.h"

namespace SE
{
	GTexture::GTexture() : SAddressable(), GShaderResourceView()
	{

	}

	GTexture::GTexture(const std::string& filePath, 
		unsigned int shaderRegisterIndex, const std::string& renderGroup) : SAddressable(), GShaderResourceView()
	{
		this->Initialize(filePath, shaderRegisterIndex, renderGroup);
	}

	GTexture::GTexture(const SUUID& assetId, const std::string& assetLoaderName)
	{
		this->Initialize(SAssetLoaderRegistry::GetInstance(assetLoaderName)->GetAsset(assetId));
	}

	GTexture::GTexture(std::shared_ptr<AAsset> asset)
	{
		this->Initialize(asset);
	}

	GTexture::GTexture(const GTexture& other) : SAddressable(other), GShaderResourceView(other)
	{
		this->TextureBuffer = other.TextureBuffer;
		this->UploadBuffer = other.UploadBuffer;
	}

	GTexture::~GTexture()
	{
		
	}

	void GTexture::Initialize(const std::string& filePath, unsigned int shaderRegisterIndex, const std::string& renderGroup)
	{
		if (GRenderGroup::CheckAvailable(renderGroup))
		{
			this->SetRootParameterIndex(shaderRegisterIndex, renderGroup);
		}
		else
		{
			this->RootParameterIndex = 0;
		}

		this->AllocateDescriptor();

		GImage TextureImage(filePath);

		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureImage.GetWidth(), TextureImage.GetHeight(), 1, 1);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
				&ResourceDesc,
				D3D12_RESOURCE_STATE_COPY_DEST, null, __uuidof(ID3D12Resource), (void**)this->TextureBuffer.GetAddressOf()));
		}

		const auto UploadBufferSize = GetRequiredIntermediateSize(this->TextureBuffer.Get(), 0, 1);
		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));
		}

		D3D12_SUBRESOURCE_DATA TextureSubresourceData;
		STELLAR_CLEAR_MEMORY(TextureSubresourceData);
		TextureSubresourceData.pData = (const void*)TextureImage.GetData();
		TextureSubresourceData.RowPitch = (long long)(((TextureImage.GetWidth() * sizeof(GColor)) + 3) & ~3);
		TextureSubresourceData.SlicePitch = (long long)TextureSubresourceData.RowPitch * TextureImage.GetHeight();
		UpdateSubresources(this->GetInitializationCommandListInstance().Get(),
			this->TextureBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &TextureSubresourceData);

		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->TextureBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
		}

		this->GetDeviceInstance()->
			CreateShaderResourceView(this->TextureBuffer.Get(), null, this->DescriptorHandle->CPUHandle);

		this->GetContext()->ExecuteInitialization();

		// Set the texture name in file name in default, which is allowed to be changed after initialization.
		this->SetName(std::filesystem::path(filePath).stem().string());

		this->Activate();
	}

	void GTexture::Initialize(std::shared_ptr<AAsset> asset)
	{
		asset->AddBindingParent(this);

		this->AllocateDescriptor();

		if (asset->GetCategory() != AAsset::Texture)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("Failed to initialize texture through the asset in a wrong category : '{}'!", asset->GetCategory()));
		}
		
		std::shared_ptr<GImage> TextureData = asset->GetData<std::shared_ptr<GImage>>();

		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureData->GetWidth(), TextureData->GetHeight(), 1, 1);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_COPY_DEST, null, __uuidof(ID3D12Resource), (void**)this->TextureBuffer.GetAddressOf()));
		}

		const auto UploadBufferSize = GetRequiredIntermediateSize(this->TextureBuffer.Get(), 0, 1);
		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));
		}

		D3D12_SUBRESOURCE_DATA TextureSubresourceData;
		STELLAR_CLEAR_MEMORY(TextureSubresourceData);
		TextureSubresourceData.pData = (const void*)TextureData->GetData();
		TextureSubresourceData.RowPitch = (long long)(((TextureData->GetWidth() * sizeof(GColor)) + 3) & ~3);
		TextureSubresourceData.SlicePitch = (long long)TextureSubresourceData.RowPitch * TextureData->GetHeight();
		UpdateSubresources(this->GetInitializationCommandListInstance().Get(),
			this->TextureBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &TextureSubresourceData);

		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->TextureBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
		}

		this->GetDeviceInstance()->
			CreateShaderResourceView(this->TextureBuffer.Get(), null, this->DescriptorHandle->CPUHandle);

		this->GetContext()->ExecuteInitialization();

		// Set the texture name in file name in default, which is allowed to be changed after initialization.
		this->SetName(asset->GetName());

		this->Activate();
		this->IsAssetized = true;
		this->AssetLoaderName = asset->GetBelongingLoaderName();
	}

	void GTexture::Apply()
	{
		GShaderResourceView::Apply();
	}

	void GTexture::ReinitializeFromAsset(AAsset* asset)
	{
		if (this->TextureBuffer)
		{
			this->TextureBuffer.Reset();
		}

		if (this->UploadBuffer)
		{
			this->UploadBuffer.Reset();
		}

		if (asset->GetCategory() != AAsset::Texture)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				std::format("Failed to initialize texture through the asset in a wrong category : '{}'!", asset->GetCategory()));
		}

		std::shared_ptr<GImage> TextureData = asset->GetData<std::shared_ptr<GImage>>();

		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, TextureData->GetWidth(), TextureData->GetHeight(), 1, 1);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_COPY_DEST, null, __uuidof(ID3D12Resource), (void**)this->TextureBuffer.GetAddressOf()));
		}

		const auto UploadBufferSize = GetRequiredIntermediateSize(this->TextureBuffer.Get(), 0, 1);
		{
			auto HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize);
			SMessageHandler::Instance->Check(this->GetDeviceInstance()->
				CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE,
					&ResourceDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ, null, __uuidof(ID3D12Resource), (void**)this->UploadBuffer.GetAddressOf()));
		}

		D3D12_SUBRESOURCE_DATA TextureSubresourceData;
		STELLAR_CLEAR_MEMORY(TextureSubresourceData);
		TextureSubresourceData.pData = (const void*)TextureData->GetData();
		TextureSubresourceData.RowPitch = (long long)(((TextureData->GetWidth() * sizeof(GColor)) + 3) & ~3);
		TextureSubresourceData.SlicePitch = (long long)TextureSubresourceData.RowPitch * TextureData->GetHeight();
		UpdateSubresources(this->GetInitializationCommandListInstance().Get(),
			this->TextureBuffer.Get(), this->UploadBuffer.Get(), 0, 0, 1, &TextureSubresourceData);

		{
			auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(this->TextureBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			this->GetInitializationCommandListInstance()->ResourceBarrier(1, &Barrier);
		}

		this->GetDeviceInstance()->
			CreateShaderResourceView(this->TextureBuffer.Get(), null, this->DescriptorHandle->CPUHandle);

		this->GetContext()->ExecuteInitialization();
	}
}
