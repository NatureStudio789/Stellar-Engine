#ifndef _SE_EDITORRENDERER_H_
#define _SE_EDITORRENDERER_H_
#include "../../Runtime/Graphics/Framebuffer/Framebuffer.h"
#include "../../Runtime/Graphics/Renderer/Renderer.h"

namespace SE
{
	class EEditorRenderer : public GRenderer
	{
	public:
		EEditorRenderer(const std::string& name);

	private:
		std::shared_ptr<GFramebuffer> EditorMainFramebuffer;
		GResourcePackage EditorMainFramebufferPackage;
	};
}

#endif