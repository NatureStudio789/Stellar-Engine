#ifndef _SE_BUFFER_H_
#define _SE_BUFFER_H_
#include "../Applicable.h"
#include "../../../Core/MessageHandler/MessageHandler.h"

namespace SE
{
	class GBuffer : public GApplicable
	{
	public:
		GBuffer();
		GBuffer(void* bufferData,
			unsigned int dataSize, unsigned int dataStride);
		GBuffer(const GBuffer& other);
		virtual ~GBuffer() override;

		void Initialize(void* bufferData, 
			unsigned int dataSize, unsigned int dataStride);

	protected:
		WRL::ComPtr<ID3DBlob> CPUBuffer;
		WRL::ComPtr<ID3D12Resource> GPUBuffer;
		WRL::ComPtr<ID3D12Resource> UploadBuffer;

		void* BufferData;
		unsigned int DataSize;
		unsigned int DataStride;
	};
}

#endif