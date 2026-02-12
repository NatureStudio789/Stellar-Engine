#ifndef _SE_MESSAGEHANDLER_H_
#define _SE_MESSAGEHANDLER_H_
#include "Message.h"

namespace SE
{
	class SMessageHandler
	{
	public:
		void SetMessage(SMessage::Level level, std::string administrator, std::string context, bool reacted = false);

		void SetInformation(std::string administrator, std::string content, bool reacted = false);
		void SetWarning(std::string administrator, std::string content, bool reacted = false);
		void SetError(std::string administrator, std::string content);
		void SetFatal(std::string administrator, std::string content);

		void Check(HRESULT hr, std::string administrator = "Graphics");

		void ShowMessagePopup(SMessage::Level level, std::string messageText);

		STELLAR_MAKE_SINGLETON(SMessageHandler, Instance);
	};
}

#endif