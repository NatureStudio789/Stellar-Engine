#ifndef _SE_FRAMEBUFFER_H_
#define _SE_FRAMEBUFFER_H_
#include "../../Core/Creatable/Creatable.h"
#include "../ContextDependent/ContextDependent.h"
#include "../SwapChain/PresentBuffer/PresentBuffer.h"
#include "../Descriptor/DescriptorHandle.h"

namespace SE
{
	class GFramebuffer : public SCreatable<GFramebuffer>, public GContextDependent
	{
	public:
		GFramebuffer();
		GFramebuffer(const glm::uvec2& size);
		GFramebuffer(std::shared_ptr<GPresentBuffer> presentBuffer);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer();

		void Initialize(const glm::uvec2& size);
		void Initialize(std::shared_ptr<GPresentBuffer> presentBuffer);

		void Clear(const glm::vec4& color);
		void Apply();

		void Begin();
		void End();

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleInstance();

		glm::uvec2 Size;

		bool IsPresentingFramebuffer;
		std::shared_ptr<GPresentBuffer> PresentRTBuffer;

		WRL::ComPtr<ID3D12Resource> RenderTargetBuffer;
		std::shared_ptr<GDescriptorHandle> RTVDescriptorHandle;

		WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;

		D3D12_VIEWPORT ViewportInstance;
		D3D12_RECT ViewportScissorRect;
	};
}

#endif