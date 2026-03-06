#ifndef _SE_TEXTURE_H_
#define _SE_TEXTURE_H_
#include "../../Core/Creatable/Creatable.h"
#include "../ShaderResourceView/ShaderResourceView.h"

namespace SE
{
	class GTexture : public GShaderResourceView, public SCreatable<GTexture>
	{
	public:

	};
}

#endif