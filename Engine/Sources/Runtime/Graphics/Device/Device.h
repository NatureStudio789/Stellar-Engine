#ifndef _SE_DEVICE_H_
#define _SE_DEVICE_H_
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/MessageHandler/MessageHandler.h"
#include "../Descriptor/DescriptorHeap.h"

namespace SE
{
	class GDevice : public SNoParameterCreatable<GDevice>
	{
	public:
		GDevice();
		GDevice(const GDevice& other);
		~GDevice();

		void Initialize();

		void ExecuteCommandLists(const std::vector<ID3D12GraphicsCommandList*>& graphicsCommandLists);

		UINT GetDescriptorSize(GDescriptorHeap::Type type);

		WRL::ComPtr<ID3D12Device> GetInstance();
		WRL::ComPtr<ID3D12CommandQueue> GetGraphicsCommandQueue();

	private:
		WRL::ComPtr<ID3D12Device> DeviceInstance;
		WRL::ComPtr<ID3D12CommandQueue> GraphicsCommandQueue;
	};
}

#endif