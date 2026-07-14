#include <Core.h>
#include "../../Runtime/Core/TimeManager/TimeManager.h"
#include "DebugEditor.h"

namespace SE
{
	EDebugEditor::EDebugEditor() : EEditor()
	{

	}

	EDebugEditor::EDebugEditor(const std::string& name) : EEditor()
	{
		this->SetName(name);
	}

	EDebugEditor::EDebugEditor(const EDebugEditor & other) : EEditor(other)
	{

	}

	EDebugEditor::~EDebugEditor()
	{

	}

	void EDebugEditor::Initialize()
	{
		
	}

	void EDebugEditor::Render()
	{
		{
			ImGui::Begin("Performance");

			ImGui::Text(("Frame Delta Time : " + std::to_string(STimeManager::GetFrameDeltaTime())).c_str());
			ImGui::Text(("FPS : " + std::to_string(STimeManager::GetFrameDeltaTime())).c_str());

			ImGui::End();
		}
	}
}
