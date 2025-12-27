#ifndef _SE_APIMANAGER_H_
#define _SE_APIMANAGER_H_
#include "../Core.h"

namespace SE
{
	class SAPIInitializer
	{
	public:
		static void InitializeAPI();

		static void ShutdownAPI();
	};
}

#endif