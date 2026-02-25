#ifndef _SE_EDITORENGINE_H_
#define _SE_EDITORENGINE_H_
#include <EngineSystem/Engine.h>
#include "Render/EditorRenderer.h"

namespace SE
{
	class EditorEngine : public SEngine
	{
	public:
		EditorEngine();

		void Initialize() override;
		void Execute() override;
		void Release() override;
	};
}

#endif