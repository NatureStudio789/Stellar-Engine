#include <Core.h>
#include "../CommandList/CommandList.h"
#include "PipelineState.h"

namespace SE
{
	GPipelineState::GPipelineState() : SAddressable()
	{
		this->RootSignature = std::make_shared<GRootSignature>();
		this->IsInitialized = false;
		
	}

	GPipelineState::GPipelineState(const std::string& name) : GPipelineState()
	{
		this->SetName(name);
	}

	GPipelineState::GPipelineState(const GPipelineState& other) : SAddressable(other)
	{
		this->PipelineStateInstance = other.PipelineStateInstance;

		this->RootSignature = other.RootSignature;

		this->ShaderList = other.ShaderList;
		this->InputLayoutInstance = other.InputLayoutInstance;
		this->Topology = other.Topology;
		this->RasterizeStateInstance = other.RasterizeStateInstance;
		this->BlendStateInstance = other.BlendStateInstance;

		this->IsInitialized = other.IsInitialized;
	}

	GPipelineState::~GPipelineState()
	{
		this->IsInitialized = false;
	}

	void GPipelineState::AddShader(std::shared_ptr<GShader> shader)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "It is NOT allowed to set this into pipeline state "
				"when pipeline state has ALREADY been initialized!");
		}

		this->ShaderList[shader->GetType()] = shader;
	}

	void GPipelineState::SetInputLayout(const InputLayout& inputLayout)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "It is NOT allowed to set this into pipeline state "
				"when pipeline state has ALREADY been initialized!");
		}

		this->InputLayoutInstance = inputLayout;
	}

	void GPipelineState::SetTopology(std::shared_ptr<GTopology> topology)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "It is NOT allowed to set this into pipeline state "
				"when pipeline state has ALREADY been initialized!");
		}

		this->Topology = topology;
	}

	void GPipelineState::SetRasterizerState(const RasterizerState& rasterizerState)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "It is NOT allowed to set this into pipeline state "
				"when pipeline state has ALREADY been initialized!");
		}

		this->RasterizeStateInstance = rasterizerState;
	}

	void GPipelineState::SetBlendState(const BlendState& blendState)
	{
		if (this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "It is NOT allowed to set this into pipeline state "
				"when pipeline state has ALREADY been initialized!");
		}

		this->BlendStateInstance = blendState;
	}

	void GPipelineState::Initialize()
	{
		this->RootSignature->Initialize();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
		STELLAR_CLEAR_MEMORY(PipelineStateDesc);

		PipelineStateDesc.pRootSignature = this->RootSignature->GetInstance().Get();
		PipelineStateDesc.InputLayout = { this->InputLayoutInstance.InputElementList.data(), (UINT)this->InputLayoutInstance.InputElementList.size()};

		PipelineStateDesc.VS =
		{
			.pShaderBytecode = this->ShaderList[GShader::SE_VERTEX_SHADER]->GetByteCode()->GetBufferPointer(),
			.BytecodeLength = this->ShaderList[GShader::SE_VERTEX_SHADER]->GetByteCode()->GetBufferSize()
		};

		PipelineStateDesc.PS =
		{
			.pShaderBytecode = this->ShaderList[GShader::SE_PIXEL_SHADER]->GetByteCode()->GetBufferPointer(),
			.BytecodeLength = this->ShaderList[GShader::SE_PIXEL_SHADER]->GetByteCode()->GetBufferSize()
		};

		auto RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		RS.CullMode = this->RasterizeStateInstance.CullMode;
		RS.FillMode = this->RasterizeStateInstance.FillMode;
		PipelineStateDesc.RasterizerState = RS;

		auto BS = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		if (this->BlendStateInstance.EnableBlend)
		{
			D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;
			STELLAR_CLEAR_MEMORY(RenderTargetBlendDesc);
			RenderTargetBlendDesc.BlendEnable = true;
			RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			RenderTargetBlendDesc.DestBlend = D3D12_BLEND_DEST_COLOR;
			RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
			RenderTargetBlendDesc.LogicOpEnable = false;
			RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_COLOR;
			RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;

			BS.RenderTarget[0] = RenderTargetBlendDesc;
		}
		PipelineStateDesc.BlendState = BS;

		PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		PipelineStateDesc.SampleMask = UINT_MAX;

		D3D12_PRIMITIVE_TOPOLOGY_TYPE Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
		if (this->Topology->GetType() == GTopology::SE_TOPOLOGY_POINTLIST)
		{
			Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		}
		else if (this->Topology->GetType() == GTopology::SE_TOPOLOGY_LINELIST ||
			this->Topology->GetType() == GTopology::SE_TOPOLOGY_LINESTRIP)
		{
			Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		}
		else if (this->Topology->GetType() == GTopology::SE_TOPOLOGY_TRIANGLELIST ||
			this->Topology->GetType() == GTopology::SE_TOPOLOGY_TRIANGLESTRIP)
		{
			Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}
		PipelineStateDesc.PrimitiveTopologyType = Type;

		PipelineStateDesc.NumRenderTargets = 1;
		PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		PipelineStateDesc.SampleDesc.Count = 1;
		PipelineStateDesc.SampleDesc.Quality = 0;
		PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		SMessageHandler::Instance->Check(this->GetDeviceInstance()->CreateGraphicsPipelineState(
			&PipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)this->PipelineStateInstance.GetAddressOf()));

		this->IsInitialized = true;

		this->Activate();
	}

	void GPipelineState::Apply()
	{
		if (!this->IsInitialized)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "The pipeline state has NOT been initialized!");
		}

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			SetGraphicsRootSignature(this->RootSignature->GetInstance().Get());

		SCommandListRegistry::GetCurrentInstance()->GetInstance()->
			SetPipelineState(this->PipelineStateInstance.Get());
	}

	WRL::ComPtr<ID3D12PipelineState> GPipelineState::GetInstance()
	{
		return this->PipelineStateInstance;
	}

	std::shared_ptr<GRootSignature> GPipelineState::GetRootSignature()
	{
		return this->RootSignature;
	}

	const bool& GPipelineState::GetInitialized() const noexcept
	{
		return this->IsInitialized;
	}


	void SPipelineStateRegistry::InitializeRegistry()
	{
		std::vector<GRootParameter> LightingRootParameterList;

		GRootParameter TransformParameter;
		TransformParameter.ParameterType = GRootParameter::SE_PARAMETER_CBV;
		TransformParameter.DescriptorCount = 1;
		TransformParameter.ShaderRegisterIndex = 0;
		LightingRootParameterList.push_back(TransformParameter);

		GRootParameter MaterialParameter;
		MaterialParameter.ParameterType = GRootParameter::SE_PARAMETER_CBV;
		MaterialParameter.DescriptorCount = 1;
		MaterialParameter.ShaderRegisterIndex = 1;
		LightingRootParameterList.push_back(MaterialParameter);

		GRootParameter CameraParameter;
		CameraParameter.ParameterType = GRootParameter::SE_PARAMETER_CBV;
		CameraParameter.DescriptorCount = 1;
		CameraParameter.ShaderRegisterIndex = 2;
		LightingRootParameterList.push_back(CameraParameter);

		GRootParameter AlbedoParameter;
		AlbedoParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
		AlbedoParameter.DescriptorCount = 1;
		AlbedoParameter.ShaderRegisterIndex = 0;
		LightingRootParameterList.push_back(AlbedoParameter);

		GRootParameter MetallicParameter;
		MetallicParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
		MetallicParameter.DescriptorCount = 1;
		MetallicParameter.ShaderRegisterIndex = 1;
		LightingRootParameterList.push_back(MetallicParameter);

		GRootParameter RoughnessParameter;
		RoughnessParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
		RoughnessParameter.DescriptorCount = 1;
		RoughnessParameter.ShaderRegisterIndex = 2;
		LightingRootParameterList.push_back(RoughnessParameter);

		GRootParameter NormalParameter;
		NormalParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
		NormalParameter.DescriptorCount = 1;
		NormalParameter.ShaderRegisterIndex = 3;
		LightingRootParameterList.push_back(NormalParameter);


		GStaticSamplerDescription DefaultSampler;
		DefaultSampler.ShaderRegister = 0;
		DefaultSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		DefaultSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		DefaultSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		DefaultSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

		// TO DO: Implement all the pipeline state which is needed.

		{
			auto AlbedoPipelineState = GPipelineState::Create(GRenderGroup::ALBEDO_GROUP);

			AlbedoPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/AlbedoVS.seshader"));
			AlbedoPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/AlbedoPS.seshader"));

			AlbedoPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			AlbedoPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> AlbedoInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			AlbedoPipelineState->SetInputLayout(GPipelineState::InputLayout(AlbedoInputLayout));

			AlbedoPipelineState->GetRootSignature()->SetParameterList(LightingRootParameterList);
			AlbedoPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			AlbedoPipelineState->Initialize();
			Register(AlbedoPipelineState);
		}

		{
			auto MetallicPipelineState = GPipelineState::Create(GRenderGroup::METALLIC_GROUP);

			MetallicPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/MetallicVS.seshader"));
			MetallicPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/MetallicPS.seshader"));

			MetallicPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			MetallicPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> MetallicInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			MetallicPipelineState->SetInputLayout(GPipelineState::InputLayout(MetallicInputLayout));

			MetallicPipelineState->GetRootSignature()->SetParameterList(LightingRootParameterList);
			MetallicPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			MetallicPipelineState->Initialize();
			Register(MetallicPipelineState);
		}

		{
			auto RoughnessPipelineState = GPipelineState::Create(GRenderGroup::ROUGHNESS_GROUP);

			RoughnessPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/RoughnessVS.seshader"));
			RoughnessPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/RoughnessPS.seshader"));

			RoughnessPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			RoughnessPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> RoughnessInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			RoughnessPipelineState->SetInputLayout(GPipelineState::InputLayout(RoughnessInputLayout));

			RoughnessPipelineState->GetRootSignature()->SetParameterList(LightingRootParameterList);
			RoughnessPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			RoughnessPipelineState->Initialize();
			Register(RoughnessPipelineState);
		}

		{
			auto NormalPipelineState = GPipelineState::Create(GRenderGroup::NORMAL_GROUP);

			NormalPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/NormalVS.seshader"));
			NormalPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/NormalPS.seshader"));

			NormalPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			NormalPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> NormalInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			NormalPipelineState->SetInputLayout(GPipelineState::InputLayout(NormalInputLayout));

			NormalPipelineState->GetRootSignature()->SetParameterList(LightingRootParameterList);
			NormalPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			NormalPipelineState->Initialize();
			Register(NormalPipelineState);
		}

		{
			auto PositionPipelineState = GPipelineState::Create(GRenderGroup::POSITION_GROUP);

			PositionPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/PositionVS.seshader"));
			PositionPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/PositionPS.seshader"));

			PositionPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			PositionPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> PositionInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			PositionPipelineState->SetInputLayout(GPipelineState::InputLayout(PositionInputLayout));

			PositionPipelineState->GetRootSignature()->SetParameterList(LightingRootParameterList);
			PositionPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			PositionPipelineState->Initialize();
			Register(PositionPipelineState);
		}

		{
			std::vector<GRootParameter> CompositionRootParameterList;

			GRootParameter AlbedoBufferParameter;
			AlbedoBufferParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
			AlbedoBufferParameter.ShaderRegisterIndex = 0;
			AlbedoBufferParameter.DescriptorCount = 1;
			CompositionRootParameterList.push_back(AlbedoBufferParameter);

			GRootParameter MetallicBufferParameter;
			MetallicBufferParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
			MetallicBufferParameter.ShaderRegisterIndex = 1;
			MetallicBufferParameter.DescriptorCount = 1;
			CompositionRootParameterList.push_back(MetallicBufferParameter);

			GRootParameter RoughnessBufferParameter;
			RoughnessBufferParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
			RoughnessBufferParameter.ShaderRegisterIndex = 2;
			RoughnessBufferParameter.DescriptorCount = 1;
			CompositionRootParameterList.push_back(RoughnessBufferParameter);

			GRootParameter NormalBufferParameter;
			NormalBufferParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
			NormalBufferParameter.ShaderRegisterIndex = 3;
			NormalBufferParameter.DescriptorCount = 1;
			CompositionRootParameterList.push_back(NormalBufferParameter);

			GRootParameter PositionBufferParameter;
			PositionBufferParameter.ParameterType = GRootParameter::SE_PARAMETER_SRV;
			PositionBufferParameter.ShaderRegisterIndex = 4;
			PositionBufferParameter.DescriptorCount = 1;
			CompositionRootParameterList.push_back(PositionBufferParameter);

			auto CompositionPipelineState = GPipelineState::Create(GRenderGroup::COMPOSITION_GROUP);

			CompositionPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/CompositionVS.seshader"));
			CompositionPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/CompositionPS.seshader"));

			CompositionPipelineState->SetTopology(GTopology::Create(GTopology::SE_TOPOLOGY_TRIANGLELIST));

			CompositionPipelineState->SetRasterizerState(GPipelineState::RasterizerState(D3D12_CULL_MODE_BACK, D3D12_FILL_MODE_SOLID));

			std::vector<D3D12_INPUT_ELEMENT_DESC> CompositionInputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			};
			CompositionPipelineState->SetInputLayout(GPipelineState::InputLayout(CompositionInputLayout));

			CompositionPipelineState->GetRootSignature()->SetParameterList(CompositionRootParameterList);
			CompositionPipelineState->GetRootSignature()->AddSamplerDescription(DefaultSampler);

			CompositionPipelineState->Initialize();
			Register(CompositionPipelineState);
		}

		{
			auto LightingPipelineState = GPipelineState::Create(GRenderGroup::LIGHTING_GROUP);

			LightingPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/LightingVS.seshader"));
			LightingPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/LightingPS.seshader"));
		}
	}
}
