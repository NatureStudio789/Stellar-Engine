#ifndef _SE_EDITORREGISTRY_H_
#define _SE_EDITORREGISTRY_H_
#include "Editor.h"

namespace SE
{
	class SEditorRegistry
	{
	public:
		static void Register(std::shared_ptr<EEditor> instance, bool isActive = true);

		static std::shared_ptr<EEditor> GetInstance(std::string name);
		static std::shared_ptr<EEditor> GetInstance(SUUID uuid);

		static bool HasInstance(std::string name);
		static bool HasInstance(SUUID uuid);

		static std::map<SUUID, std::shared_ptr<EEditor>> GetInstanceList();

		static void Initialize();

		static void SetEditorActive(const SUUID& uuid, bool isActive);
		static void SetEditorActive(const std::string& name, bool isActive);

		static bool GetEditorActive(const SUUID& uuid);
		static bool GetEditorActive(const std::string& name);

	private:
		static std::map<SUUID, std::shared_ptr<EEditor>> RegisteredInstanceList;
		static std::map<SUUID, bool> EditorStateList;
	};
}

#endif