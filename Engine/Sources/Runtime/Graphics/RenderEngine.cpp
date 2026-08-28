#include <Core.h>
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"
#include "PipelineState/PipelineState.h"
#include "Material/StandardMaterial.h"
#include "../Function/Input/Input.h"
#include "../Core/TimeManager/TimeManager.h"
#include "RenderEngine.h"

#include "../Core/APIConfigurator/APIConfigurator.h"

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
		
		this->TestLoader = std::make_shared<AAssetLoader>();
		this->TestLoader->SetName("Test");
		SAssetLoaderRegistry::Register(this->TestLoader);
		std::thread Loading([this]()
			{
				try 
				{
					this->TestLoader->Initialize("Engine/Assets/");
				}
				catch (const SE::SMessage& message)
				{
					(void)message;
					SE::SAPIConfigurator::ShutdownAPI();

					return -1;
				}
				catch (const std::filesystem::filesystem_error& e) {
					// 捕获异常
					::MessageBoxA(null, e.what(), "Stellar Engine - filesystem", MB_OK | MB_ICONINFORMATION);

					return -1;
				}

				return 0;
			});
		Loading.join();

		this->TestCamera = std::make_shared<GCamera>();
		this->TestCamera->SetName("Test");
		this->TestCamera->SetPosition(0.0f, 10.0f, -2.0f);
		this->TestCamera->SetNearZ(0.01f);
		this->TestCamera->SetFarZ(300.0f);
		this->TestCamera->IsFreeLook = true;
		SCameraRegistry::Register(this->TestCamera);

		auto DeferredRenderer = std::make_shared<GDeferredRenderer>("MainDeferredRenderer");
		DeferredRenderer->SetMainCamera(this->TestCamera->GetUUID());
		DeferredRenderer->Compile();
		SRendererRegistry::Register(DeferredRenderer);
		FMouse::AddMovementCallback([this](const glm::ivec2& movement)
		{
			this->TestCamera->Rotate({ movement.y * 0.008f * STimeManager::GetFrameDeltaTime(), movement.x * 0.008f * STimeManager::GetFrameDeltaTime(), 0.0f});
		});

		this->TestPLRegistry = std::make_shared<GPointLightRegistry>();
		DeferredRenderer->SetLightRegistry(TestPLRegistry);
		this->TestDLRegistry = std::make_shared<GDirectionalLightRegistry>("MainDirectionalLightRegistry");
		DeferredRenderer->SetLightRegistry(TestDLRegistry);

		//std::vector<std::shared_ptr<GPointLight>> TestLightList;
		//TestLightList.resize(20);

		//std::random_device rd;
		//std::mt19937 gen(rd());
		//std::uniform_real_distribution<float> PositionXZDist(-80.0f, 80.0f);
		//std::uniform_real_distribution<float> ColorDist(0.0f, 1.0f);

		//int i = 0;
		//for (auto light : TestLightList)
		//{
		//	light = std::make_shared<GPointLight>("point_light_test" + std::to_string(i), GPointLight::Data({PositionXZDist(gen), 20.0f, PositionXZDist(gen)}, 600.0f,
		//		{ ColorDist(gen), ColorDist(gen), ColorDist(gen)}));
		//	this->TestPLRegistry->Register(light);

		//	i++;
		//}

		glm::vec3 direction = { 1.0f, 0.0f, 0.0f };
		glm::quat rot = { glm::radians(glm::vec3(0.0f, 0.0f, -50.0f)) };
		direction = glm::normalize(rot * direction);
		std::shared_ptr<GDirectionalLight> DirectionalLight = std::make_shared<GDirectionalLight>("test2", 
			GDirectionalLight::Data(direction, 10.0f, {1.0f, 1.0f, 0.8f}));
		this->TestDLRegistry->Register(DirectionalLight);

		{
			this->testmesh = std::make_shared<GStaticMesh>(this->TestLoader->GetAsset("Engine/Assets/Models/Terrain/Terrain.sasset"));
			this->testmesh->SetTransform(STransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)), glm::vec3(5.0f, 5.0f, 5.0f)));
		}

		{
			this->TestMeshTransform.Position = { 0.0f, 18.0f, 0.0f };
			this->TestMeshTransform.Scale = { 4.0f, 4.0f, 4.0f };
			this->TestMeshTransform.Rotation = glm::quat(glm::radians(glm::vec3{0.0f, 0.0f, 0.0f}));
			this->testmesh2 = std::make_shared<GStaticMesh>(this->TestLoader->GetAsset("Engine/Assets/Models/Cerberus/Cerberus.sasset"));
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
			this->TestCamera->Translate(this->TestCamera->Forward * 0.03f * STimeManager::GetFrameDeltaTime());
		}
		if (FKeyboard::GetKeyPress('S'))
		{
			this->TestCamera->Translate(-this->TestCamera->Forward * 0.03f * STimeManager::GetFrameDeltaTime());
		}
		if (FKeyboard::GetKeyPress('A'))
		{
			this->TestCamera->Translate(-this->TestCamera->Right * 0.03f * STimeManager::GetFrameDeltaTime());
		}
		if (FKeyboard::GetKeyPress('D'))
		{
			this->TestCamera->Translate(this->TestCamera->Right * 0.03f * STimeManager::GetFrameDeltaTime());
		}

		this->TestMeshTransform.Rotate({ 0.0f, -0.08f * STimeManager::GetFrameDeltaTime(), 0.0f});

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