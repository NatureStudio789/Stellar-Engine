#ifndef _SE_DESCRIPTORHEAP_H_
#define _SE_DESCRIPTORHEAP_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/MessageHandler/MessageHandler.h"
#include "Allocator/DescriptorAllocator.h"
#include "DescriptorHandle.h"

namespace SE
{
	class GDevice;

	class GDescriptorHeap : public SCreatable<GDescriptorHeap>
	{
	public:
		enum Type
		{
			SE_DESCRIPTORHEAP_CBVSRVUAV = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			SE_DESCRIPTORHEAP_SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			SE_DESCRIPTORHEAP_RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			SE_DESCRIPTORHEAP_DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};

		enum Flag
		{
			SE_DESCRIPTORHEAP_FLAG_NONE = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			SE_DESCRIPTORHEAP_FLAG_SHADERVISIBLE = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		};

	public:
		GDescriptorHeap();
		GDescriptorHeap(std::shared_ptr<GDevice> device, unsigned int descriptorCount,
			Type type, Flag flag = SE_DESCRIPTORHEAP_FLAG_NONE);
		GDescriptorHeap(const GDescriptorHeap& other);
		~GDescriptorHeap();

		void Initialize(std::shared_ptr<GDevice> device, unsigned int descriptorCount,
			Type type, Flag flag = SE_DESCRIPTORHEAP_FLAG_NONE);

		std::shared_ptr<GDescriptorHandle> Allocate(UINT count = 1);

		const Type& GetDescriptorType() const noexcept;
		WRL::ComPtr<ID3D12DescriptorHeap> GetInstance();
		const UINT& GetIncrementSize() const noexcept;

	private:
		Type DescriptorType;
		UINT IncrementSize;
		
		WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapInstance;
		std::shared_ptr<GDescriptorAllocator> Allocator;
	};
}

#endif