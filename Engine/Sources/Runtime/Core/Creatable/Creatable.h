#ifndef _SE_CREATABLE_H_
#define _SE_CREATABLE_H_

namespace SE
{
	template<typename CreateType>
	class SCreatable
	{
	public:
		template<typename... Args>
		static std::shared_ptr<CreateType> Create(Args&&... args)
		{
			return std::make_shared<CreateType>(std::forward<Args>(args)...);
		}
	};

	template<typename CreateType>
	class SNoParameterCreatable
	{
	public:
		static std::shared_ptr<CreateType> Create()
		{
			auto instance = std::make_shared<CreateType>();
			instance->Initialize();

			return instance;
		}
	};
}

#endif