#ifndef _SE_PRESENTBUFFER_H_
#define _SE_PRESENTBUFFER_H_

namespace SE
{
	class GPresentBuffer
	{
	public:
		GPresentBuffer();
		GPresentBuffer(const GPresentBuffer& other);
		~GPresentBuffer();

		void ResetBuffer();

		WRL::ComPtr<ID3D12Resource> GetCurrentBuffer();

		glm::uvec2 Size;
		unsigned int Count;
		std::vector<WRL::ComPtr<ID3D12Resource>> BufferList;
		unsigned int CurrentBufferIndex;
	};
}

#endif