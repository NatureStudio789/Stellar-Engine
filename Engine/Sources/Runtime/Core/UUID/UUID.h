#ifndef _SE_UUID_H_
#define _SE_UUID_H_
#include "Generator/UUIDGenerator.h"

namespace SE
{
	class SUUID
	{
	public:
		SUUID();
		SUUID(const SUUID& other);
		~SUUID();

		void Activate();
		void Destroy();

		const std::string& operator()() const noexcept;
		bool operator<(const SUUID& other) const;

		bool GetAvailable() const noexcept;

	private:
		std::string UUID;
		bool IsAvailable;
	};
}

#endif