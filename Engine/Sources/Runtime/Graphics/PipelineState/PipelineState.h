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
			InputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementList, unsigned int elementCount) :
				InputElementList(inputElementList), ElementCount(elementCount)
			{

			}
			InputLayout(const InputLayout& ) = default;

			D3D12_INPUT_ELEMENT_DESC* InputElementList;
			unsigned int ElementCount;
		};

		struct RasterizerState
		{
			RasterizerState() = default;
			RasterizerState(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode) :
				CullMode(cullMode), FillMode(fillMode)
			{

			}
			RasterizerState(const RasterizerState&) = default;

			D3D12_CULL_MODE CullMode;
			D3D12_FILL_MODE FillMode;
		};

		struct BlendState
		{
			BlendState() = default;
			BlendState(bool enableBlend) : EnableBlend(enableBlend)
			{

			}
			BlendState(const BlendState&) = default;

			bool EnableBlend;
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

		bool IsInitialized;
	};

	class SPipelineStateRegistry : public SRegistry<GPipelineState>
	{
	public:
		static void InitializeRegistry();
	};
}

#endif