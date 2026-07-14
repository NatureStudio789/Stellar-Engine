#ifndef _SE_DEBUGEDITOR_H_
#define _SE_DEBUGEDITOR_H_
#include "../Editor.h"

namespace SE
{
	class EDebugEditor : public EEditor
	{
	public:
		EDebugEditor();
		EDebugEditor(const std::string& name);
		EDebugEditor(const EDebugEditor& other);
		~EDebugEditor() override;

		void Initialize() override;
		void Render() override;
	};
}

#endif