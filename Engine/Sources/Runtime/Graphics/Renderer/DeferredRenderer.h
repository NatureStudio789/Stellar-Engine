#ifndef _SE_DEFERREDRENDERER_H_
#define _SE_DEFERREDRENDERER_H_
#include "Renderer.h"

namespace SE
{
	class GCompositionPass;
	class GPointLightRegistry;

	class GDeferredRenderer : public GRenderer
	{
	public:
		GDeferredRenderer(const std::string& name);

		/*Normally, There is only one light registry for one category available in renderer.
		If the function is called twice, the data of the exist one would be replace.*/
		void SetLightRegistry(std::shared_ptr<GPointLightRegistry> registry);

	private:
		std::shared_ptr<GFramebuffer> GBufferFramebuffer;
		std::shared_ptr<GFramebuffer> FinalCompositionFramebuffer;

		std::shared_ptr<GCompositionPass> CompositionPass;
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