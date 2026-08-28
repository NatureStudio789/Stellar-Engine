#ifndef _SE_TEXTURE_H_
#define _SE_TEXTURE_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Assetizable/Assetizable.h"

#include "../Image/Image.h"
#include "../ShaderResourceView/ShaderResourceView.h"

namespace SE
{
	class AAsset;

	class GTexture : public SAddressable, public GShaderResourceView, 
		public SCreatable<GTexture>, public SAssetizable
	{
	public:
		GTexture();
		GTexture(const std::string& filePath, 
			unsigned int shaderRegisterIndex = 0, const std::string& renderGroup = "");
		GTexture(const SUUID& assetId, const std::string& assetLoaderName);
		GTexture(std::shared_ptr<AAsset> asset);
		GTexture(const GTexture& other);
		~GTexture() override;

		void Initialize(const std::string& filePath,
			unsigned int shaderRegisterIndex = 0, const std::string& renderGroup = "");
		void Initialize(std::shared_ptr<AAsset> asset);

		void Apply() override;

	private:
		void ReinitializeFromAsset(AAsset* asset) override;

		//std::string BelongRenderGoup;

		WRL::ComPtr<ID3D12Resource> TextureBuffer;
		WRL::ComPtr<ID3D12Resource> UploadBuffer;
	};
}

#endif