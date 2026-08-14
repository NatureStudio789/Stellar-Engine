#include <Core.h>

#include "../../Core/MessageHandler/MessageHandler.h"

#include "DirectoryObserver.h"

namespace SE
{
	FDirectoryObserver::FDirectoryObserver()
	{
		this->DirectoryHandle = INVALID_HANDLE_VALUE;
		this->IOCP = null;

		this->IsWorking = false;
	}

	FDirectoryObserver::~FDirectoryObserver()
	{
		this->Stop();
	}

	void FDirectoryObserver::Start(const std::string & directoryPath, Callback callbackFunction)
	{
		this->DirectoryHandle = CreateFileW(std::wstring(directoryPath.begin(), directoryPath.end()).c_str(), FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, null, OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, null);
		if (this->DirectoryHandle == INVALID_HANDLE_VALUE)
		{
			SMessageHandler::Instance->SetFatal("Function",
				std::format("Failed to create handle for directory : '{}'!", directoryPath));
		}

		this->IOCP = CreateIoCompletionPort(this->DirectoryHandle, null, 0, 0);
		if (!this->IOCP)
		{
			CloseHandle(this->IOCP);
			this->DirectoryHandle = INVALID_HANDLE_VALUE;
			SMessageHandler::Instance->SetFatal("Function", "Failed to create IO Completion Port!");
		}

		this->CallbackFunction = std::move(callbackFunction);
		this->IsWorking = true;

		this->ObervationWorker = std::thread(&FDirectoryObserver::Observe, this);

		this->PostReadOperation();
	}

	void FDirectoryObserver::Stop()
	{
		if (this->DirectoryHandle != INVALID_HANDLE_VALUE)
		{
			this->IsWorking = false;

			CancelIo(this->DirectoryHandle);
			if (this->ObervationWorker.joinable())
			{
				this->ObervationWorker.join();
			}

			CloseHandle(this->DirectoryHandle);
			this->DirectoryHandle = INVALID_HANDLE_VALUE;
			if (this->IOCP)
			{
				CloseHandle(this->IOCP);
				this->IOCP = null;
			}
		}
	}

	void FDirectoryObserver::Observe()
	{
		DWORD BytesTransferred;
		ULONG_PTR CompletionKey;
		OVERLAPPED* OverlappedPtr;

		while (this->IsWorking)
		{
			bool success = GetQueuedCompletionStatus(this->IOCP,
				&BytesTransferred, &CompletionKey, &OverlappedPtr, INFINITE);

			if (!this->IsWorking || !success)
			{
				break;
			}

			if (OverlappedPtr == null)
			{
				continue;
			}

			if (BytesTransferred > 0)
			{
				this->ProcessNotification(this->Buffer, BytesTransferred);
			}

			if (IsWorking)
			{
				this->PostReadOperation();
			}
		}
	}

	void FDirectoryObserver::PostReadOperation()
	{
		STELLAR_CLEAR_MEMORY(this->Overlapped);
		ZeroMemory(&this->Buffer, BUFFER_SIZE);

		bool success = ReadDirectoryChangesW(this->DirectoryHandle,
											 this->Buffer,
											 BUFFER_SIZE,
											 true,
											 FILE_NOTIFY_CHANGE_FILE_NAME |
											 FILE_NOTIFY_CHANGE_DIR_NAME |
											 FILE_NOTIFY_CHANGE_ATTRIBUTES |
											 FILE_NOTIFY_CHANGE_SIZE |
											 FILE_NOTIFY_CHANGE_LAST_WRITE |
											 FILE_NOTIFY_CHANGE_LAST_ACCESS |
											 FILE_NOTIFY_CHANGE_CREATION |
											 FILE_NOTIFY_CHANGE_SECURITY,
											 null,
											 &this->Overlapped,
											 null);

		if (!success && GetLastError() != ERROR_IO_PENDING)
		{
			IsWorking = false;
		}
	}

	void FDirectoryObserver::ProcessNotification(BYTE* buffer, DWORD size)
	{
		if (size == 0)
		{
			return;
		}

		BYTE* p = buffer;
		for (;;)
		{
			FILE_NOTIFY_INFORMATION* FNI = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(p);

			std::wstring FileName(FNI->FileName, FNI->FileNameLength / sizeof(WCHAR));
			if (CallbackFunction)
			{
				CallbackFunction(FNI->Action, FileName);
			}

			if (FNI->NextEntryOffset == 0)
			{
				break;
			}
			p += FNI->NextEntryOffset;
		}
	}
}
