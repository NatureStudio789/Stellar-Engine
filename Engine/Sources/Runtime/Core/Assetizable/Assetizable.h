#ifndef _SE_ASSETIZABLE_H_
#define _SE_ASSETIZABLE_H_

namespace SE
{
	class AAsset;

	class SAssetizable
	{
	public:
		SAssetizable();
		SAssetizable(const SAssetizable& other);
		virtual ~SAssetizable();

		bool GetAssetized() const noexcept;

	protected:
		virtual void ReinitializeFromAsset(AAsset*) = 0;

		std::string AssetLoaderName;
		bool IsAssetized = false;

		friend class AAsset;
	};
}

#endif