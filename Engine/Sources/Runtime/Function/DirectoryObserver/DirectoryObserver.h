#ifndef _SE_DIRECTORYOBSERVER_H_
#define _SE_DIRECTORYOBSERVER_H_

namespace SE
{
	class FDirectoryObserver
	{
	public:
		using Callback = std::function<void(DWORD action, const std::wstring& fileName)>;

	public:
		FDirectoryObserver();
		~FDirectoryObserver();

		void Start(const std::string& directoryPath, Callback callbackFunctionInWorkerThread);

		void Stop();

	private:
		void Observe();
		void PostReadOperation();
		void ProcessNotification(BYTE* buffer, DWORD size);

		static constexpr DWORD BUFFER_SIZE = 65536; // 64 KB

		HANDLE DirectoryHandle;
		HANDLE IOCP;
		
		std::atomic<bool> IsWorking;

		std::thread ObervationWorker;
		Callback CallbackFunction;

		OVERLAPPED Overlapped;
		BYTE Buffer[BUFFER_SIZE];
	};
}

#endif