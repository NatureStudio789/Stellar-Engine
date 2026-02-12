#include <Core.h>
#include "Message.h"

namespace SE
{
	SMessage::SMessage()
	{
		this->IsAvailable = false;
	}

	SMessage::SMessage(Level level, std::string administrator, std::string content)
	{
		this->Initialize(level, administrator, content);
	}

	SMessage::SMessage(const SMessage& other)
	{
		this->MessageLevel = other.MessageLevel;
		this->Administrator = other.Administrator;
		this->Content = other.Content;
		this->Text = other.Text;

		this->IsAvailable = other.IsAvailable;
	}

	SMessage::~SMessage()
	{
		this->IsAvailable = false;
	}

	void SMessage::Initialize(Level level, std::string administrator, std::string content)
	{
		switch (level)
		{
			case SE_MESSAGE_INFO:
			{
				this->MessageLevel = level;
				this->Administrator = administrator;
				this->Content = content;

				this->Text = "[" + this->Administrator + "]" + " <-> " + "Information : \n    " + this->Content;

				this->IsAvailable = true;

				break;
			}

			case SE_MESSAGE_WARNING:
			{
				this->MessageLevel = level;
				this->Administrator = administrator;
				this->Content = content;

				this->Text = "[" + this->Administrator + "]" + " <-> " + "Wanring : \n    " + this->Content;

				this->IsAvailable = true;

				break;
			}

			case SE_MESSAGE_ERROR:
			{
				this->MessageLevel = level;
				this->Administrator = administrator;
				this->Content = content;

				this->Text = "[" + this->Administrator + "]" + " <-> " + "Error : \n    " + this->Content;

				this->IsAvailable = true;

				break;
			}

			case SE_MESSAGE_FATAL:
			{
				this->MessageLevel = level;
				this->Administrator = administrator;
				this->Content = content;

				this->Text = "[" + this->Administrator + "]" + " <-> " + "Fatal : \n    " + this->Content;

				this->IsAvailable = true;

				break;
			}

			default:
			{
				this->IsAvailable = false;
				break;
			}
		}
	}

	bool SMessage::GetMessageAvailable() const noexcept
	{
		return this->IsAvailable;
	}

	const SMessage::Level& SMessage::GetLevel() const noexcept
	{
		return this->MessageLevel;
	}

	const std::string& SMessage::GetAdministrator() const noexcept
	{
		return this->Administrator;
	}

	const std::string& SMessage::GetContent() const noexcept
	{
		return this->Content;
	}

	const std::string& SMessage::GetText() const noexcept
	{
		return this->Text;
	}
}
