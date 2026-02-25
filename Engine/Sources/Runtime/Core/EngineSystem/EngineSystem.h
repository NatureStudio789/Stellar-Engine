#ifndef _SE_ENGINESYSTEM_H_
#define _SE_ENGINESYSTEM_H_

namespace SE
{
	class SEngine;

	class SEngineSystem
	{
	public:
		static void InitializeAll();
		static void ExecuteAll();
		static void ReleaseAll();

		static void RegisterEngine(std::unique_ptr<SEngine> engine);

	private:
		static std::map<std::string, std::unique_ptr<SEngine>>& GetEngineList();
	};
}

#endif