#ifndef _SE_RENDERCONFIGURATION_H_
#define _SE_RENDERCONFIGURATION_H_

namespace SE
{
	class GRenderConfiguration
	{
	public:
		enum RenderingParadigmCategory
		{
			SE_FORWARD_RENDERING,
			SE_DEFERRED_RENDERING
		};

	public:
		static void InitializeConfiguration();

		static const RenderingParadigmCategory& GetRenderingParadigm();

	private:
		static RenderingParadigmCategory RenderingParadigm;
	};
}

#endif