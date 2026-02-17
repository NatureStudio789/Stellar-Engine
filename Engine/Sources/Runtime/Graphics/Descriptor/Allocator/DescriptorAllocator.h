#ifndef _SE_DESCRIPTORALLOCATOR_H_
#define _SE_DESCRIPTORALLOCATOR_H_
#include "../../../Core/Creatable/Creatable.h"
#include "../../../Core/MessageHandler/MessageHandler.h"

namespace SE
{
	class GDescriptorAllocator : public SCreatable<GDescriptorAllocator>
	{
	public:
		GDescriptorAllocator();
		GDescriptorAllocator(unsigned long long count);
		GDescriptorAllocator(const GDescriptorAllocator& other);
		~GDescriptorAllocator();

		void Initialize(unsigned long long count);

		unsigned int Allocate(unsigned long long count);
		void Release(unsigned long long start, unsigned long long count);

		unsigned long long GetDescriptorCount() const noexcept;
		unsigned long long GetAllocatedCount() const;
		unsigned long long GetFreeCount() const;

	private:
		bool IsBitSet(unsigned long long index) const;
		void SetBit(unsigned long long index);
		void ClearBit(unsigned long long index);
		void MarkAllocated(unsigned long long start, unsigned long long end);

		unsigned long long DescriptorCount;
		std::vector<unsigned long long> Bitmap;
	};
}

#endif