#ifndef _SE_TEXTURE_H_
#define _SE_TEXTURE_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/Addressable/Addressable.h"

#include "../Image/Image.h"
#include "../ShaderResourceView/ShaderResourceView.h"

namespace SE
{
	class GTexture : public SAddressable, public GShaderResourceView, public SCreatable<GTexture>
	{
	public:
		GTexture();
		GTexture(const std::string& filePath, 
			unsigned int shaderRegisterIndex = 0, const std::string& renderGroup = "");
		GTexture(const GTexture& other);
		~GTexture() override;

		void Initialize(const std::string& filePath,
			unsigned int shaderRegisterIndex = 0, const std::string& renderGroup = "");

		void Apply() override;

	private:
		std::string BelongRenderGoup;

		WRL::ComPtr<ID3D12Resource> TextureBuffer;
		WRL::ComPtr<ID3D12Resource> UploadBuffer;
	};
}

#endif