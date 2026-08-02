#ifndef _SE_PIPELINESTATE_H_
#define _SE_PIPELINESTATE_H_
#include "../Applicable/Applicable.h"
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Registry/Registry.h"

#include "../Shader/Shader.h"
#include "../Topology/Topology.h"

#include "../RootSignature/RootSignature.h"

#include "../RenderGroup/RenderGroup.h"

namespace SE
{
	class GPipelineState : public GApplicable, public SAddressable, public SCreatable<GPipelineState>
	{
	public:
		struct InputLayout
		{
			InputLayout() = default;
			InputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementList) : InputElementList(inputElementList)
			{

			}
			InputLayout(const InputLayout& ) = default;

			std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementList;
		};

		struct RasterizerState
		{
			RasterizerState()
			{
				this->DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
				this->DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
				this->SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

				this->CullMode = D3D12_CULL_MODE_NONE;
				this->FillMode = D3D12_FILL_MODE_SOLID;
			}
			RasterizerState(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode, 
				int depthBias = D3D12_DEFAULT_DEPTH_BIAS, float depthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP, 
				float slopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS) :
				CullMode(cullMode), FillMode(fillMode), DepthBias(depthBias), DepthBiasClamp(depthBiasClamp), SlopeScaledDepthBias(slopeScaledDepthBias)
			{

			}
			RasterizerState(const RasterizerState&) = default;

			INT DepthBias;
			FLOAT DepthBiasClamp;
			FLOAT SlopeScaledDepthBias;
			D3D12_CULL_MODE CullMode;
			D3D12_FILL_MODE FillMode;
		};

		struct BlendState
		{
			BlendState()
			{
				this->EnableBlend = false;
			}
			BlendState(bool enableBlend) : EnableBlend(enableBlend)
			{

			}
			BlendState(const BlendState&) = default;

			bool EnableBlend;
		};

		struct RenderTargetConfiguration
		{
			RenderTargetConfiguration()
			{
				this->RenderTargetCount = 1;
				this->RenderTargetFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			unsigned int RenderTargetCount;
			DXGI_FORMAT RenderTargetFormat[8];
		};

	public:
		GPipelineState();
		GPipelineState(const std::string& name);
		GPipelineState(const GPipelineState& other);
		~GPipelineState();

		void AddShader(std::shared_ptr<GShader> shader);

		void SetInputLayout(const InputLayout& inputLayout);
		void SetTopology(std::shared_ptr<GTopology> topology);
		void SetRasterizerState(const RasterizerState& rasterizerState);
		void SetBlendState(const BlendState& blendState);
		void SetRenderTargetConfiguration(const RenderTargetConfiguration& renderTargetConfiguration);

		void Initialize();

		void Apply() override;

		WRL::ComPtr<ID3D12PipelineState> GetInstance();
		std::shared_ptr<GRootSignature> GetRootSignature();
		const bool& GetInitialized() const noexcept;

	private:
		WRL::ComPtr<ID3D12PipelineState> PipelineStateInstance;

		std::shared_ptr<GRootSignature> RootSignature;

		std::map<GShader::Type, std::shared_ptr<GShader>> ShaderList;
		InputLayout InputLayoutInstance;
		std::shared_ptr<GTopology> Topology;
		RasterizerState RasterizeStateInstance;
		BlendState BlendStateInstance;
		RenderTargetConfiguration RenderTargetConfigurationInstance;

		bool IsInitialized;
	};

	class SPipelineStateRegistry : public SRegistry<GPipelineState>
	{
	public:
		static void InitializeRegistry();
	};
}

#endif