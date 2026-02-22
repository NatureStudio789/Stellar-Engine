#ifndef _SE_COMMANDLIST_H_
#define _SE_COMMANDLIST_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Creatable/Creatable.h"
#include "../../Core/Registry/Registry.h"

namespace SE
{
	class GDevice;

	class GCommandList : public SCreatable<GCommandList>, public SAddressable
	{
	public:
		enum Type
		{
			SE_COMMAND_LIST_DIRECT = D3D12_COMMAND_LIST_TYPE_DIRECT,
			SE_COMMAND_LIST_BUNDLE = D3D12_COMMAND_LIST_TYPE_BUNDLE,
			SE_COMMAND_LIST_COMPUTE = D3D12_COMMAND_LIST_TYPE_COMPUTE,
			SE_COMMAND_LIST_COPY = D3D12_COMMAND_LIST_TYPE_COPY,
			SE_COMMAND_LIST_VIDEO_DECODE = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
			SE_COMMAND_LIST_VIDEO_PROCESS = D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS,
			SE_COMMAND_LIST_VIDEO_ENCODE = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE,
		};

	public:
		GCommandList();
		GCommandList(std::shared_ptr<GDevice> device, Type type);
		GCommandList(const GCommandList& other);
		~GCommandList();

		void Initialize(std::shared_ptr<GDevice> device, Type type);

		void Open();
		void Close();

		WRL::ComPtr<ID3D12GraphicsCommandList> GetInstance();

	private:
		Type CommandListType;

		WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
		WRL::ComPtr<ID3D12GraphicsCommandList> CommandListInstance;
	};

	STELLAR_MAKE_EXECUTABLE_REGISTRY(GCommandList, CommandListRegistry);
}

#endif