#ifndef _SE_APICONFIGURATOR_H_
#define _SE_APICONFIGURATOR_H_

namespace SE
{
	class SAPIConfigurator
	{
	public:
		static void InitializeAPI();
		static void PostInitializeAPI();

		static void ShutdownAPI();
	};
}

#endif