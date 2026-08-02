#ifndef _SE_SRVSET_H_
#define _SE_SRVSET_H_
#include "../../Core/Addressable/Addressable.h"

#include "../ContextDependent/ContextDependent.h"
#include "ShaderResourceView.h"
#include "../Renderer/ResourcePackage/ResourcePackage.h"

namespace SE
{
	/*A SET holds a bunch of allocated SRVs which are in series, in order to apply 'em 
	to pipeline as a array. Several independent Framebuffers, Depth Maps or Textures etc. could 
	get allocated SRV for initialization from the set so that they could be applied in one array together.
	l-----------------------------------------------------------------------------------------------------l
	*TIPS* : In general, AppendSRV() should be called per frame to overcome the dynamic situation.*/
	class GSRVSet : public GApplicable, public SAddressable
	{
	public:
		GSRVSet();
		GSRVSet(const std::string& name, unsigned int size);
		GSRVSet(const GSRVSet& other);
		~GSRVSet() override;

		void Initialize(const std::string& name, unsigned int size);

		//This function should be called per frame to overcome the dynamic situation.
		void AppendSRV(GShaderResourceView* srv);
		/*Set current member count to zero for updating sort state per frame. 
		It can be called after appending all required SRV members, which is because SRVs 
		all have been set in proper place in set array.*/
		void ResetMemberState();

		void SetRootParameterIndex(unsigned int index);

		void Apply() override;

		unsigned int GetSize() const noexcept;
		std::shared_ptr<GDescriptorHandle> GetDescriptorHandle();
		unsigned int GetRootParameterIndex() const noexcept;

		GResourcePackage GetResourcePackage() const noexcept;

		bool GetDescriptorAllocated() const noexcept;

	private:
		unsigned int Size;
		std::shared_ptr<GDescriptorHandle> DescriptorHandle;
		/*This member is for noting the count of SRV members in current render round(per frame). 
		It will be set to zero when calling ResetMemberState().*/
		unsigned int CurrentMemberCount;
		unsigned int RootParameterIndex;

		bool IsDescriptorAllocated;
	};

	STELLAR_MAKE_BLANK_REGISTRY(GSRVSet, SRVSetRegistry);
}

#endif