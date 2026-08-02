#ifndef _SE_DEPTHMAP_H_
#define _SE_DEPTHMAP_H_
#include "../../../Core/Addressable/Addressable.h"
#include "../../../Core/Registry/Registry.h"
	
#include "../../ContextDependent/ContextDependent.h"
#include "../../ShaderResourceView/ShaderResourceView.h"
#include "../../Renderer/ResourcePackage/ResourcePackage.h"

namespace SE
{
	class GDepthMap : public GContextDependent, public SAddressable
	{
	public:
		GDepthMap();
		GDepthMap(const glm::uvec2& size);
		GDepthMap(const GDepthMap& other);
		~GDepthMap() override;

		void Initialize(const glm::uvec2& size);

		void Resize(const glm::uvec2& newSize);

		void Begin();
		void Clear();
		void Apply();
		void End();

		/*This function is used when SRV is reallocated or modified.*/
		void RebindSRV();

		std::shared_ptr<GShaderResourceView> GetShaderResourceView();
		GResourcePackage GetResourcePackage() const noexcept;
		const glm::uvec2& GetSize() const noexcept;
		unsigned int GetWidth() const noexcept;
		unsigned int GetHeight() const noexcept;

	private:
		glm::uvec2 Size;

		WRL::ComPtr<ID3D12Resource> DepthMapBuffer;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;
		std::shared_ptr<GShaderResourceView> DepthMapSRV;

		D3D12_VIEWPORT ViewportInstance;
		D3D12_RECT ViewportScissorRect;
	};

	STELLAR_MAKE_BLANK_REGISTRY(GDepthMap, DepthMapRegistry);
}

#endif