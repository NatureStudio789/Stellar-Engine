#include "Core.h"
#include "RenderConfiguration.h"

namespace SE
{
	GRenderConfiguration::RenderingParadigmCategory GRenderConfiguration::RenderingParadigm;


	void GRenderConfiguration::InitializeConfiguration()
	{
		RenderingParadigm = SE_DEFERRED_RENDERING;
	}

	const GRenderConfiguration::RenderingParadigmCategory& GRenderConfiguration::GetRenderingParadigm()
	{
		return RenderingParadigm;
	}
}
