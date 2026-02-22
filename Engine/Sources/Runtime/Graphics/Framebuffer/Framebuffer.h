#ifndef _SE_FRAMEBUFFER_H_
#define _SE_FRAMEBUFFER_H_
#include "../../Core/Creatable/Creatable.h"
#include "../ContextDependent/ContextDependent.h"
#include "../SwapChain/SwapChain.h"
#include "../Descriptor/DescriptorHandle.h"
#include "../../Core/Registry/Registry.h"

namespace SE
{
	class GFramebuffer : public SCreatable<GFramebuffer>, public GContextDependent, public SAddressable
	{
	public:
		GFramebuffer();
		GFramebuffer(const glm::uvec2& size);
		GFramebuffer(std::shared_ptr<GSwapChain> bufferSwapChain);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer();

		void Initialize(const glm::uvec2& size);
		void Initialize(std::shared_ptr<GSwapChain> bufferSwapChain);

		void Clear(const glm::vec4& color);
		void Apply();
		void Resize(const glm::uvec2& newSize);

		void Begin();
		void End();

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleInstance();

		glm::uvec2 Size;

		bool IsPresentingFramebuffer;
		std::shared_ptr<GSwapChain> RTBufferSwapChain;

		WRL::ComPtr<ID3D12Resource> RenderTargetBuffer;
		std::shared_ptr<GDescriptorHandle> RTVDescriptorHandle;

		WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;

		D3D12_VIEWPORT ViewportInstance;
		D3D12_RECT ViewportScissorRect;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GFramebuffer, FramebufferRegistry);
}

#endif