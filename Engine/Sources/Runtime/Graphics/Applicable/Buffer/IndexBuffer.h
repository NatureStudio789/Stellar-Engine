#ifndef _SE_INDEXBUFFER_H_
#define _SE_INDEXBUFFER_H_
#include "Buffer.h"
#include "../../../Core/Creatable/Creatable.h"

namespace SE
{
	class GIndexBuffer : public GBuffer, public SCreatable<GIndexBuffer>
	{
	public:
		GIndexBuffer();
		GIndexBuffer(void* bufferData, unsigned int dataSize, unsigned int dataStride);
		GIndexBuffer(const GIndexBuffer& other);

		void Apply() override;

		const unsigned int& GetIndicesCount() const noexcept;

	private:

	};
}

#endif