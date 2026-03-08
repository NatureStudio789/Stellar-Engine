#ifndef _SE_SHADER_H_
#define _SE_SHADER_H_
#include "../../Core/Creatable/Creatable.h"
#include "../ContextDependent/ContextDependent.h"
#include "../../Core/MessageHandler/MessageHandler.h"

namespace SE
{
	class GShader : public GContextDependent, public SCreatable<GShader>
	{
	public:
		enum Type
		{
			SE_VERTEX_SHADER,
			SE_PIXEL_SHADER,
			SE_DOMAIN_SHADER,
			SE_HULL_SHADER,
			SE_GEOMETRY_SHADER,
		};

	public:
		GShader();
		GShader(Type type, const std::string& filePath, bool isFileCompiled = false);
		GShader(const GShader& other);
		~GShader();

		void Initialize(Type type, const std::string& filePath, bool isFileCompiled = false);

		const Type& GetType() const noexcept;
		WRL::ComPtr<ID3DBlob> GetByteCode();

	private:
		Type ShaderType;
		WRL::ComPtr<ID3DBlob> CompiledByteCode;
	};
}

#endif