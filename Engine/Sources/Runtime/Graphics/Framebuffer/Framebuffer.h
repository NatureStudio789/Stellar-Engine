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
		GFramebuffer(const glm::uvec2& size, unsigned int multipleRenderTargetCount = 1,
			std::vector<DXGI_FORMAT> renderTargetFormat = std::vector<DXGI_FORMAT>{ DXGI_FORMAT_R8G8B8A8_UNORM });
		GFramebuffer(std::shared_ptr<GSwapChain> bufferSwapChain);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer();

		void Initialize(const glm::uvec2& size, unsigned int multipleRenderTargetCount = 1,
			std::vector<DXGI_FORMAT> renderTargetFormat = std::vector<DXGI_FORMAT>{ DXGI_FORMAT_R8G8B8A8_UNORM });
		void Initialize(std::shared_ptr<GSwapChain> bufferSwapChain);

		void Resize(const glm::uvec2& newSize);

		void SetCurrentBuffer(unsigned int multipleRenderTargetBufferIndex);
		void Begin();
		void BeginMultiple(const std::vector<unsigned int>& bufferIndices);
		void Clear(const glm::vec4& color);
		void Apply();
		void End();
		void EndMultiple(const std::vector<unsigned int>& bufferIndices);

		std::shared_ptr<GShaderResourceView> GetRTShaderResourceView(unsigned int multipleRenderTargetBufferIndex = 0);
		GResourcePackage GetResourcePackage() const noexcept;
		const glm::uvec2& GetSize() const noexcept;
		unsigned int GetWidth() const noexcept;
		unsigned int GetHeight() const noexcept;

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVDescriptorHandleInstance();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVDescriptorHandleInstance();

		glm::uvec2 Size;

		bool IsPresentingFramebuffer;
		std::shared_ptr<GSwapChain> RTBufferSwapChain;

		std::vector<WRL::ComPtr<ID3D12Resource>> RenderTargetBufferList;
		std::vector<DXGI_FORMAT> RenderTargetFormat;
		std::vector<std::shared_ptr<GShaderResourceView>> RTShaderResourceViewList;
		unsigned int CurrentBufferIndex;
		std::shared_ptr<GDescriptorHandle> RTVDescriptorHandle;

		std::vector<WRL::ComPtr<ID3D12Resource>> DepthStencilBufferList;
		std::shared_ptr<GDescriptorHandle> DSVDescriptorHandle;

		/*Resources queued for deferred destruction after GPU fence completion.
		Resize() moves old RT/DS resources here instead of immediately releasing them
		so they stay alive until the GPU finishes executing command lists that
		reference them (prevents D3D12 ERROR #921: OBJECT_DELETED_WHILE_STILL_IN_USE).*/
		std::vector<WRL::ComPtr<ID3D12Resource>> PendingReleaseRT;
		std::vector<WRL::ComPtr<ID3D12Resource>> PendingReleaseDS;

		void ReleasePendingResources();

		D3D12_VIEWPORT ViewportInstance;
		D3D12_RECT ViewportScissorRect;
	};

	STELLAR_MAKE_DEFAULT_REGISTRY(GFramebuffer, FramebufferRegistry);
}

#endif