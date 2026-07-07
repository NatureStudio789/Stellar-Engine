#ifndef _SE_VIEWPORTEDITOR_H_
#define _SE_VIEWPORTEDITOR_H_
#include "../Editor.h"

namespace SE
{
	class EViewportEditor : public EEditor
	{
	public:
		EViewportEditor();
		EViewportEditor(const std::string& name);
		EViewportEditor(const EViewportEditor& other);
		~EViewportEditor() override;

		void Initialize() override;
		void Render() override;
	};
}

#endif