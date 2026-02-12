#include <Core.h>
#include "MessageHandler.h"

namespace SE
{
	STELLAR_FINE_SINGLETON(SMessageHandler, Instance);
	
	
	void SMessageHandler::SetMessage(SMessage::Level level, std::string administrator, std::string context, bool reacted)
	{
		SMessage Message = SMessage(level, administrator, context);

		if (reacted)
		{
			ShowMessagePopup(Message.GetLevel(), Message.GetText());

			if (Message.GetLevel() == SMessage::SE_MESSAGE_ERROR || 
				Message.GetLevel() == SMessage::SE_MESSAGE_FATAL)
			{
				throw Message;
			}
		}
	}

	void SMessageHandler::SetInformation(std::string administrator, std::string content, bool reacted)
	{
		this->SetMessage(SMessage::SE_MESSAGE_INFO, administrator, content, reacted);
	}

	void SMessageHandler::SetWarning(std::string administrator, std::string content, bool reacted)
	{
		this->SetMessage(SMessage::SE_MESSAGE_WARNING, administrator, content, reacted);
	}

	void SMessageHandler::SetError(std::string administrator, std::string content)
	{
		this->SetMessage(SMessage::SE_MESSAGE_ERROR, administrator, content, true);
	}

	void SMessageHandler::SetFatal(std::string administrator, std::string content)
	{
		this->SetMessage(SMessage::SE_MESSAGE_FATAL, administrator, content, true);
	}

	void SMessageHandler::Check(HRESULT hr, std::string administrator)
	{
		if (hr == S_OK || hr == S_FALSE)
		{
			return;
		}

		std::string ErrorString;
		char* MessageBuffer = null;
		unsigned long MessageLength = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			null, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&MessageBuffer), 0, null);
		if (MessageLength == 0)
		{
			ErrorString = "Undefined Error Code!";
		}
		ErrorString = MessageBuffer;

		LocalFree(MessageBuffer);

		this->SetFatal(administrator, ErrorString);
	}

	void SMessageHandler::ShowMessagePopup(SMessage::Level level, std::string messageText)
	{
		switch (level)
		{
			case SMessage::SE_MESSAGE_INFO:
			{
				::MessageBoxA(null, messageText.c_str(), "Stellar Engine - Information", MB_OK | MB_ICONINFORMATION);
				break;
			}

			case SMessage::SE_MESSAGE_WARNING:
			{
				::MessageBoxA(null, messageText.c_str(), "Stellar Engine - Warning", MB_OK | MB_ICONWARNING);
				break;
			}

			case SMessage::SE_MESSAGE_ERROR:
			{
				::MessageBoxA(null, messageText.c_str(), "Stellar Engine - Error", MB_OK | MB_ICONERROR);
				break;
			}

			case SMessage::SE_MESSAGE_FATAL:
			{
				::MessageBoxA(null, messageText.c_str(), "Stellar Engine - Fatal", MB_OK | MB_ICONSTOP);
				break;
			}

			default:
			{
				::MessageBoxA(null, messageText.c_str(), "Stellar Engine - Unknown", MB_OK | MB_ICONQUESTION);
				break;
			}
		}
	}
}
