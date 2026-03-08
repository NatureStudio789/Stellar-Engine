#include <Core.h>
#include "../CommandList/CommandList.h"
#include "PipelineState.h"

namespace SE
{
	GPipelineState::GPipelineState()
	{
		this->RootSignature = std::make_shared<GRootSignature>();
		this->IsInitialized = false;
	}

	GPipelineState::GPipelineState(const std::string& name) : GPipelineState()
	{
		this->SetName(name);
	}

	GPipelineState::GPipelineState(const GPipelineState& other)
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
		PipelineStateDesc.InputLayout = { this->InputLayoutInstance.InputElementList, this->InputLayoutInstance.ElementCount };

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
		// TO DO: Implement all the pipeline state which is needed.

		{
			auto LightingPipelineState = GPipelineState::Create(GRenderGroup::LIGHTING_GROUP);

			LightingPipelineState->AddShader(GShader::Create(GShader::SE_VERTEX_SHADER, "Engine/Shaders/LightingVS.seshader"));
			LightingPipelineState->AddShader(GShader::Create(GShader::SE_PIXEL_SHADER, "Engine/Shaders/LightingPS.seshader"));

		}
	}
}
