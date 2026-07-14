#include <Core.h>
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"
#include "PipelineState/PipelineState.h"
#include "Material/StandardMaterial.h"
#include "../Function/Input/Input.h"
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
		this->TestCamera->SetPosition(0.0f, 0.0f, 16.0f);
		this->TestCamera->SetNearZ(0.01f);
		this->TestCamera->SetFarZ(10000.0f);
		this->TestCamera->IsFreeLook = true;
		SCameraRegistry::Register(this->TestCamera);

		auto DeferredRenderer = std::make_shared<GDeferredRenderer>("MainDeferredRenderer");
		DeferredRenderer->Compile();
		DeferredRenderer->SetMainCamera(this->TestCamera->GetName());
		SRendererRegistry::Register(DeferredRenderer);
		FMouse::AddMovementCallback([this](const glm::ivec2& movement)
		{
			this->TestCamera->Rotate({ -movement.y * 0.1f, movement.x * 0.1f, 0.0f });
		});

		this->TestPLRegistry = std::make_shared<GPointLightRegistry>();
		DeferredRenderer->SetLightRegistry(TestPLRegistry);
		this->TestDLRegistry = std::make_shared<GDirectionalLightRegistry>();
		DeferredRenderer->SetLightRegistry(TestDLRegistry);

		std::vector<std::shared_ptr<GPointLight>> TestLightList;
		TestLightList.resize(20);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> PositionXZDist(-600.0f, 600.0f);
		std::uniform_real_distribution<float> ColorDist(0.0f, 1.0f);

		for (auto light : TestLightList)
		{
			light = std::make_shared<GPointLight>("test", GPointLight::Data({ PositionXZDist(gen), 30.0f, PositionXZDist (gen)}, 60000.0f,
				{ ColorDist(gen), ColorDist(gen), ColorDist(gen)}));
			//this->TestPLRegistry->Register(light);
		}

		glm::vec3 direction = { 0.0f, 0.0f, 1.0f };
		glm::quat rot = { glm::radians(glm::vec3(-45.0f, 0.0f, 0.0f)) };
		direction = rot * direction;
		std::shared_ptr<GDirectionalLight> DirectionalLight = std::make_shared<GDirectionalLight>("test2", 
			GDirectionalLight::Data(direction, 30.0f, {1.0f, 1.0f, 0.8f}));
		this->TestDLRegistry->Register(DirectionalLight);

		std::shared_ptr<GStandardMaterial> testmat = std::make_shared<GStandardMaterial>("testmat");
		SMaterialRegistry::Register(testmat);
		testmat->SetAlbedo(GTexture::Create("Engine/Assets/Textures/Background.jpeg", 0, GRenderGroup::ALBEDO_GROUP));

		{
			this->testmesh = std::make_shared<GStaticMesh>("Engine/Assets/Models/Sponza/Sponza.fbx");
		}

		{
			this->TestMeshTransform.Position = { 0.0f, 30.0f, 0.0f };
			this->TestMeshTransform.Scale = { 8.0f, 8.0f, 8.0f };
			this->testmesh2 = std::make_shared<GStaticMesh>("Engine/Assets/Models/Cerberus/Cerberus.fbx");
			this->testmesh2->SetTransform(this->TestMeshTransform);
		}
	}

	void RenderEngine::Execute()
	{
		// Wait for the oldest in-flight frame to complete before recording new commands.
		SGraphicsContextRegistry::GetMainInstance()->GetSwapChain()->WaitForFrameFence();


		this->testmesh->Submit("main");
		this->testmesh2->Submit("main");

		if (FKeyboard::GetKeyPress('W'))
		{
			this->TestCamera->Translate(this->TestCamera->Forward * 0.5f);
		}
		if (FKeyboard::GetKeyPress('S'))
		{
			this->TestCamera->Translate(-this->TestCamera->Forward * 0.5f);
		}
		if (FKeyboard::GetKeyPress('A'))
		{
			this->TestCamera->Translate(-this->TestCamera->Right * 0.5f);
		}
		if (FKeyboard::GetKeyPress('D'))
		{
			this->TestCamera->Translate(this->TestCamera->Right * 0.5f);
		}

		this->TestMeshTransform.Rotate({ 0.0f, -0.8f, 0.0f });

		this->testmesh2->SetTransform(this->TestMeshTransform);


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

		SGraphicsContextRegistry::GetMainInstance()->Present(1);

		// Signal that this frame's GPU work has been submitted; advance to next frame.
		SGraphicsContextRegistry::GetMainInstance()->GetSwapChain()->MoveToNextFrame();
	}

	void RenderEngine::Release()
	{
		/*Flush the GPU command queue to ensure all in - flight operations complete
		before resources are released during engine shutdown. This prevents
		D3D12 ERROR #921: OBJECT_DELETED_WHILE_STILL_IN_USE.*/
		this->MainGraphicsContext->Flush();
	}
}