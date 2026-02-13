#include <Core.h>
#include "Device.h"

namespace SE
{
	GDevice::GDevice()
	{

	}

	GDevice::GDevice(std::shared_ptr<GGraphicsFactory> factory)
	{
		this->Initialize(factory);
	}

	GDevice::GDevice(const GDevice& other)
	{
		this->DeviceInstance = other.DeviceInstance;
	}

	GDevice::~GDevice()
	{

	}

	void GDevice::Initialize(std::shared_ptr<GGraphicsFactory> factory)
	{
		for (UINT index = 0;
			factory->GetInstance()->EnumAdapters1(index, this->Adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND;
			index++)
		{
			DXGI_ADAPTER_DESC1 AdapterDesc;
			STELLAR_CLEAR_MEMORY(AdapterDesc);

			this->Adapter->GetDesc1(&AdapterDesc);

			if (AdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(this->Adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), null)))
			{
				break;
			}
		}

		SMessageHandler::Instance->Check(D3D12CreateDevice(this->Adapter.Get(), D3D_FEATURE_LEVEL_12_0,
			__uuidof(ID3D12Device), (void**)this->DeviceInstance.GetAddressOf()));
	}

	WRL::ComPtr<ID3D12Device> GDevice::GetInstance()
	{
		return this->DeviceInstance;
	}
}
