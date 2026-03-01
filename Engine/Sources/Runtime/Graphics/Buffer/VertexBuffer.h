#ifndef _SE_VERTEXBUFFER_H_
#define _SE_VERTEXBUFFER_H_
#include "../../Core/Creatable/Creatable.h"
#include "Buffer.h"

namespace SE
{
	class GVertexBuffer : public GBuffer, public SCreatable<GVertexBuffer>
	{
	public:
		GVertexBuffer();
		GVertexBuffer(void* bufferData,
			unsigned int dataSize, unsigned int dataStride);
		GVertexBuffer(const GVertexBuffer& other);

		void Apply() override;

		const unsigned int& GetVerticesCount() const noexcept;

	private:

	};
}

#endif