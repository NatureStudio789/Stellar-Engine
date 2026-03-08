#include <Core.h>
#include "Shader.h"

namespace SE
{
	GShader::GShader() : GContextDependent()
	{

	}

	GShader::GShader(Type type, const std::string& filePath, bool isFileCompiled) : GContextDependent()
	{
		this->Initialize(type, filePath, isFileCompiled);
	}

	GShader::GShader(const GShader& other) : GContextDependent(other)
	{
		this->ShaderType = other.ShaderType;
		this->CompiledByteCode = other.CompiledByteCode;
	}

	GShader::~GShader()
	{

	}

	void GShader::Initialize(Type type, const std::string& filePath, bool isFileCompiled)
	{
		this->ShaderType = type;

		if (isFileCompiled)
		{
			std::ifstream ShaderFileStream(filePath, std::ios::binary);

			ShaderFileStream.seekg(0, std::ios::end);
			std::ifstream::pos_type size = ShaderFileStream.tellg();
			ShaderFileStream.seekg(0, std::ios::beg);

			SMessageHandler::Instance->Check(D3DCreateBlob((SIZE_T)size, this->CompiledByteCode.GetAddressOf()));

			ShaderFileStream.read((char*)this->CompiledByteCode->GetBufferPointer(), (std::streamsize)size);
			ShaderFileStream.close();
		}
		else
		{
			std::string CompileTarget;
			switch (this->ShaderType)
			{
				case SE_VERTEX_SHADER:
				{
					CompileTarget = "vs_5_0";

					break;
				}

				case SE_PIXEL_SHADER:
				{
					CompileTarget = "ps_5_0";

					break;
				}

				case SE_DOMAIN_SHADER:
				{
					CompileTarget = "ds_5_0";

					break;
				}

				case SE_HULL_SHADER:
				{
					CompileTarget = "hs_5_0";

					break;
				}

				case SE_GEOMETRY_SHADER:
				{
					CompileTarget = "gs_5_0";

					break;
				}

				default:
				{
					CompileTarget = "unknown_0_0";

					break;
				}
			}

			unsigned int CompileFlags = 0;
#ifdef SE_DEBUG
			CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			WRL::ComPtr<ID3DBlob> CompileResult;
			WRL::ComPtr<ID3DBlob> ErrorBuffer;
			HRESULT hr = D3DCompileFromFile(std::wstring(filePath.begin(), filePath.end()).c_str(), null,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", CompileTarget.c_str(), CompileFlags, 0,
				CompileResult.GetAddressOf(), ErrorBuffer.GetAddressOf());
			if (FAILED(hr))
			{
#ifdef SE_DEBUG
				::OutputDebugStringA((char*)ErrorBuffer->GetBufferPointer());
#endif
			}

			SMessageHandler::Instance->Check(hr);

			this->CompiledByteCode = CompileResult;
		}
	}

	const GShader::Type& GShader::GetType() const noexcept
	{
		return this->ShaderType;
	}

	WRL::ComPtr<ID3DBlob> GShader::GetByteCode()
	{
		return this->CompiledByteCode;
	}
}
