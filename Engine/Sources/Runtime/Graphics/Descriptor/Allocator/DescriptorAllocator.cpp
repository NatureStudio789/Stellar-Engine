#include <Core.h>
#include "DescriptorAllocator.h"

namespace SE
{
	GDescriptorAllocator::GDescriptorAllocator()
	{
		this->DescriptorCount = 0;
		this->Bitmap.clear();
	}

	GDescriptorAllocator::GDescriptorAllocator(unsigned long long count)
	{
		this->Initialize(count);
	}

	GDescriptorAllocator::GDescriptorAllocator(const GDescriptorAllocator& other)
	{
		this->DescriptorCount = other.DescriptorCount;
		this->Bitmap = other.Bitmap;
	}

	GDescriptorAllocator::~GDescriptorAllocator()
	{
		this->DescriptorCount = 0;
		this->Bitmap.clear();
	}

	void GDescriptorAllocator::Initialize(unsigned long long count)
	{
		this->DescriptorCount = count;

		this->Bitmap.resize((this->DescriptorCount + 63) / 64, 0);
	}

	unsigned int GDescriptorAllocator::Allocate(unsigned long long count)
	{
		if (this->DescriptorCount < count)
		{
			SMessageHandler::Instance->SetFatal("Graphics",
				"The count of descriptor allocation request is out of the total count!");
		}

		unsigned long long CurrentFreeCount = 0;
		unsigned long long StartIndex = 0;

		for (unsigned long long i = 0; i < this->DescriptorCount; i++)
		{
			if (!this->IsBitSet(i))
			{
				CurrentFreeCount++;

				if (CurrentFreeCount == count)
				{
					this->MarkAllocated(StartIndex, i);
					return (UINT)StartIndex;
				}
			}
			else
			{
				CurrentFreeCount = 0;
				StartIndex = i + 1;
			}
		}

		SMessageHandler::Instance->SetFatal("Graphics", "No more enough free decriptors for allocation");
		return 0;
	}

	void GDescriptorAllocator::Release(unsigned long long start, unsigned long long count)
	{
		if (start >= this->DescriptorCount || count == 0 || (start + count) > this->DescriptorCount)
		{
			SMessageHandler::Instance->SetFatal("Graphics", "The releasing range is invalid!");
		}

		for (unsigned long long i = start; i < (start + count); i++)
		{
			this->ClearBit(i);
		}
	}

	unsigned long long GDescriptorAllocator::GetDescriptorCount() const noexcept
	{
		return this->DescriptorCount;
	}

	unsigned long long GDescriptorAllocator::GetAllocatedCount() const
	{
		unsigned long long Count = 0;
		for (const auto& block : this->Bitmap)
		{
			Count += __popcnt64(block);
		}

		return Count;
	}

	unsigned long long GDescriptorAllocator::GetFreeCount() const
	{
		return this->DescriptorCount - this->GetAllocatedCount();
	}

	bool GDescriptorAllocator::IsBitSet(unsigned long long index) const
	{
		unsigned long long BlockIndex = index / 64;
		unsigned long long BitOffset = index % 64;

		return (this->Bitmap[BlockIndex] & (1ull << BitOffset)) != 0;
	}

	void GDescriptorAllocator::SetBit(unsigned long long index)
	{
		unsigned long long BlockIndex = index / 64;
		unsigned long long BitOffset = index % 64;

		this->Bitmap[BlockIndex] |= (1ull << BitOffset);
	}

	void GDescriptorAllocator::ClearBit(unsigned long long index)
	{
		unsigned long long BlockIndex = index / 64;
		unsigned long long BitOffset = index % 64;

		this->Bitmap[BlockIndex] &= ~(1ull << BitOffset);
	}

	void GDescriptorAllocator::MarkAllocated(unsigned long long start, unsigned long long end)
	{
		for (unsigned long long i = start; i <= end; i++)
		{
			this->SetBit(i);
		}
	}
}
