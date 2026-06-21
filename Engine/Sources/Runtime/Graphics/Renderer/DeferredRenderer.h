#ifndef _SE_DEFERREDRENDERER_H_
#define _SE_DEFERREDRENDERER_H_
#include "Renderer.h"

namespace SE
{
	class GDeferredRenderer : public GRenderer
	{
	public:
		GDeferredRenderer(const std::string& name);



	private:
		std::shared_ptr<GFramebuffer> GBufferFramebuffer;
		std::shared_ptr<GFramebuffer> FinalCompositionFramebuffer;
	};
}

/*
Deferred Renderer Passes Framebuffer Indexes:
	Albedo : 0
	Metallic : 1
	Roughness : 2
	Normal : 3
*/

#endif