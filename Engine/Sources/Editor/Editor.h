#ifndef _SE_EDITOR_H_
#define _SE_EDITOR_H_
#include "../Runtime/Core/Addressable/Addressable.h"

namespace SE
{
	class EEditor : public SAddressable
	{
	public:
		EEditor();
		EEditor(const EEditor& other);
		virtual ~EEditor() override;

		virtual void Initialize();
		virtual void Render();

	protected:
	};
}

#endif