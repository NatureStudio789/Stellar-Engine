#ifndef _SE_FRAMEBUFFER_H_
#define _SE_FRAMEBUFFER_H_
#include "../../Core/Creatable/Creatable.h"
#include "../ContextDependent/ContextDependent.h"
#include "../SwapChain/SwapChain.h"
#include "../Descriptor/DescriptorHandle.h"
#include "../../Core/Registry/Registry.h"
#include "../ShaderResourceView/ShaderResourceView.h"

namespace SE
{
	class GResourcePackage;

	class GFramebuffer : public SCreatable<GFramebuffer>, public GContextDependent, public SAddressable
	{
	public:
		GFramebuffer();
		GFramebuffer(const glm::uvec2& size, unsigned int multipleRenderTargetCount = 1);
		GFramebuffer(std::shared_ptr<GSwapChain> bufferSwapChain);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer();

		void Initialize(const glm::uvec2& size, unsigned int multipleRenderTargetCount = 1);
		void Initialize(std::shared_ptr<GSwapChain> bufferSwapChain);

		void Resize(const glm::uvec2& newSize);

		void SetCurrentBuffer(unsigned int multipleRenderTargetBufferIndex);
		void Begin();
		void Clear(const glm::vec4& color);
		void Apply();
		void End();

		std::shared_ptr<GShaderResourceView> GetRTShaderResourceView(unsigned int multipleRenderTargetBufferIndex);
		GResourcePackage GetResourcePackage() const noexcept;

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleInstance();

		glm::uvec2 Size;

		bool IsPresentingFramebuffer;
		std::shared_ptr<GSwapChain> RTBufferSwapChain;

		std::vector<WRL::ComPtr<ID3D12Resource>> RenderTargetBufferList;
		std::vector<std::shared_ptr<GShaderResourceView>> RTShaderResourceViewList;
		unsigned int CurrentBufferIndex;
		std::shared_ptr<GDescriptorHandle> RTVDescriptorHandle;

		WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;

		D3D12_VIEWPORT ViewportInstance;
		D3D12_RECT ViewportScissorRect;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GFramebuffer, FramebufferRegistry);
}

#endif