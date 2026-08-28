#ifndef _SE_ASSETSERIALIZER_H_
#define _SE_ASSETSERIALIZER_H_

namespace SE
{
	class AAsset;

	class FAssetSerializer
	{
	public:
		static void Serialize(const AAsset* asset, YAML::Emitter& serializationData);
		static void Deserialize(AAsset* asset, YAML::Node& deserializationData);

		// Import the serialized data of asset.
		static void Import(std::string filePath, AAsset* asset);
		// Export the serialization data of asset.
		static void Export(std::string filePath, AAsset* asset);

		// Load tree of serialized data from file.
		static YAML::Node Load(std::string filePath);
	};
}

#endif	