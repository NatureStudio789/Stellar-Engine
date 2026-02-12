#ifndef _SE_MESSAGE_H_
#define _SE_MESSAGE_H_

namespace SE
{
	class SMessage
	{
	public:
		enum Level
		{
			SE_MESSAGE_INFO,
			SE_MESSAGE_WARNING,
			SE_MESSAGE_ERROR,
			SE_MESSAGE_FATAL
		};

	public:
		SMessage();
		SMessage(const SMessage& other);
		~SMessage();

		bool GetMessageAvailable() const noexcept;

		const Level& GetLevel() const noexcept;
		const std::string& GetAdministrator() const noexcept;
		const std::string& GetContent() const noexcept;
		const std::string& GetText() const noexcept;

	private:
		SMessage(Level level, std::string administrator, std::string content);
		void Initialize(Level level, std::string administrator, std::string content);

		Level MessageLevel;
		std::string Administrator;
		std::string Content;
		std::string Text;

		bool IsAvailable;

		friend class SMessageHandler;
	};
}

#endif