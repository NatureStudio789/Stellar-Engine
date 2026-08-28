#include "Core.h"
#include "Assetizable.h"

namespace SE
{
    SAssetizable::SAssetizable()
    {
        this->AssetLoaderName = "";
        this->IsAssetized = false;
    }

    SAssetizable::SAssetizable(const SAssetizable& other)
    {
        this->AssetLoaderName = other.AssetLoaderName;
        this->IsAssetized = other.IsAssetized;
    }

    SAssetizable::~SAssetizable()
    {
        this->IsAssetized = false;
    }

    bool SAssetizable::GetAssetized() const noexcept
    {
        return this->IsAssetized;
    }
}
