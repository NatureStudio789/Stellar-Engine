#include <Core.h>
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"
#include "PipelineState/PipelineState.h"
#include "Material/StandardMaterial.h"
#include "../Function/Input/Keyboard/Keyboard.h"
#include "RenderEngine.h"

namespace SE
{
	STELLAR_REGISTER_ENGINE(RenderEngine);
	

	RenderEngine::RenderEngine()
	{
		this->EngineName = "#1_Render_Engine";
	}

	void RenderEngine::Initialize()
	{
		GRenderConfiguration::InitializeConfiguration();

		this->MainGraphicsContext = GGraphicsContext::Create(
			SWindowRegistry::GetMainInstance()->GetWindowHandle(),
			SWindowRegistry::GetMainInstance()->GetWindowSize());
		this->MainGraphicsContext->SetName(SGraphicsContextRegistry::MainInstanceName);
		SGraphicsContextRegistry::Register(this->MainGraphicsContext);

		SPipelineStateRegistry::InitializeRegistry();

		this->TestCamera = std::make_shared<GCamera>();
		this->TestCamera->SetName("Test");
		this->TestCamera->SetPosition(0.0f, 0.0f, 18.0f);
		this->TestCamera->SetNearZ(0.01f);
		this->TestCamera->SetFarZ(100.0f);
		SCameraRegistry::Register(this->TestCamera);

		auto& DeferredRenderer = std::make_shared<GDeferredRenderer>("MainDeferredRenderer");
		DeferredRenderer->Compile();
		DeferredRenderer->SetMainCamera(this->TestCamera->GetName());
		SRendererRegistry::Register(DeferredRenderer);

		std::shared_ptr<GStandardMaterial> testmat = std::make_shared<GStandardMaterial>("testmat");
		SMaterialRegistry::Register(testmat);
		testmat->SetAlbedo(GTexture::Create("Engine/Assets/Textures/Background.jpeg", 0, GRenderGroup::ALBEDO_GROUP));

		//{
		//	GMeshItem::Data data;
		//	data.Name = "test";
		//	data.Vertices =
		//	{
		//		GMeshItem::Vertex{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
		//		GMeshItem::Vertex{{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
		//		GMeshItem::Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}
		//	};
		//	data.Indices = { 0, 1, 2 };
		//	data.MaterialId = testmat->GetUUID();
		//	this->test = std::make_shared<GMeshItem>(data);

		//	this->test->LinkTechnique("MainDeferredRenderer");
		//}

		//{
		//	GMeshItem::Data data;
		//	data.Name = "test1";
		//	data.Vertices =
		//	{
		//		GMeshItem::Vertex{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
		//		GMeshItem::Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
		//		GMeshItem::Vertex{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
		//	};
		//	data.Indices = { 0, 1, 2 };
		//	data.MaterialId = testmat->GetUUID();
		//	this->test1 = std::make_shared<GMeshItem>(data);

		//	this->test1->LinkTechnique("MainDeferredRenderer");
		//}

		{
			this->TestMeshTransform.SetRotation({ 0.0f, 180.0f, 0.0f });
			this->testmesh = std::make_shared<GStaticMesh>("Engine/Assets/Models/Nanosuit/Nanosuit.fbx");
		}
	}

	void RenderEngine::Execute()
	{
		// Wait for the oldest in-flight frame to complete before recording new commands.
		SGraphicsContextRegistry::GetMainInstance()->GetSwapChain()->WaitForFrameFence();

		//this->test->Submit("main");
		//this->test1->Submit("main");
		this->testmesh->Submit("main");

		//if (FKeyboard::GetKeyPress('W'))
		//{
		//	this->TestCamera->Translate(0.0f, 0.0f, -0.1f);
		//}
		//if (FKeyboard::GetKeyPress('S'))
		//{
		//	this->TestCamera->Translate(0.0f, 0.0f, 0.1f);
		//}
		//if (FKeyboard::GetKeyPress('A'))
		//{
		//	this->TestCamera->Translate(0.1f, 0.0f, 0.0f);
		//}
		//if (FKeyboard::GetKeyPress('D'))
		//{
		//	this->TestCamera->Translate(-0.1f, 0.0f, 0.0f);
		//}
		//if (FKeyboard::GetKeyPress('Q'))
		//{
		//	this->TestCamera->Rotate({ 0.0f, -2.0f, 0.0f });
		//}
		//if (FKeyboard::GetKeyPress('E'))
		//{
		//	this->TestCamera->Rotate({ 0.0f, 2.0f, 0.0f });
		//}

		//if (FKeyboard::GetKeyPress('Z'))
		//{
		//	this->TestMeshTransform.Rotate({ 0.0f, -1.0f, 0.0f });
		//}
		//if (FKeyboard::GetKeyPress('X'))
		//{
		//	this->TestMeshTransform.Rotate({ 0.0f, 1.0f, 0.0f });
		//}

		this->TestMeshTransform.Rotate({ 0.0f, -1.0f, 0.0f });

		this->testmesh->SetTransform(this->TestMeshTransform);

		for (auto& [uuid, renderer] : SRendererRegistry::GetInstanceList())
		{
			renderer->Execute();
		}

		std::vector<ID3D12GraphicsCommandList*> ExecutingCommandList;
		for (auto& [uuid, commandList] : SCommandListRegistry::GetInstanceList())
		{
			ExecutingCommandList.push_back(commandList->GetInstance().Get());
		}
		SGraphicsContextRegistry::GetMainInstance()->ExecuteCommandLists(ExecutingCommandList);

		SGraphicsContextRegistry::GetMainInstance()->Present(true);

		// Signal that this frame's GPU work has been submitted; advance to next frame.
		SGraphicsContextRegistry::GetMainInstance()->GetSwapChain()->MoveToNextFrame();
	}

	void RenderEngine::Release()
	{
		// Flush the GPU command queue to ensure all in-flight operations complete
		// before resources are released during engine shutdown. This prevents
		// D3D12 ERROR #921: OBJECT_DELETED_WHILE_STILL_IN_USE.
		this->MainGraphicsContext->Flush();
	}
}