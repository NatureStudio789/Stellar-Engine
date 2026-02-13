#ifndef _SE_DEVICE_H_
#define _SE_DEVICE_H_
#include "../Factory/GraphicsFactory.h"

namespace SE
{
	class GDevice : public SCreatable<GDevice>
	{
	public:
		GDevice();
		GDevice(std::shared_ptr<GGraphicsFactory> factory);
		GDevice(const GDevice& other);
		~GDevice();

		void Initialize(std::shared_ptr<GGraphicsFactory> factory);

		WRL::ComPtr<ID3D12Device> GetInstance();

	private:
		WRL::ComPtr<IDXGIAdapter1> Adapter;
		WRL::ComPtr<ID3D12Device> DeviceInstance;
	};
}

#endif