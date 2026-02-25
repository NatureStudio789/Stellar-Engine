#include <Core.h>
#include "EditorEngine.h"

namespace SE
{
	STELLAR_REGISTER_ENGINE(EditorEngine);

	EditorEngine::EditorEngine()
	{
		this->EngineName = "#2_Editor_Engine";
	}

	void EditorEngine::Initialize()
	{
		auto EditorRenderer = std::make_shared<EEditorRenderer>("EditorMainRenderer");
		SRendererRegistry::Register(EditorRenderer);
	}

	void EditorEngine::Execute()
	{

	}

	void EditorEngine::Release()
	{

	}
}
