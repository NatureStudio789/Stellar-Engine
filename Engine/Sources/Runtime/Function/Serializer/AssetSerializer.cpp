#include <Core.h>

#include "../../Asset/Asset.h"

#include "AssetSerializer.h"

namespace SE
{
	void FAssetSerializer::Serialize(const AAsset* asset, YAML::Emitter& serializationData)
	{
		auto& SerializationData = serializationData;

		SerializationData << YAML::BeginMap;

		{
			SerializationData << YAML::Key << "Asset";
			SerializationData << YAML::Value << (const std::string&)asset->GetUUID();
			SerializationData << YAML::Key << "Name";
			SerializationData << YAML::Value << asset->GetName();
			SerializationData << YAML::Key << "Category";
			SerializationData << YAML::Value << asset->GetCategory();

			SerializationData << YAML::Key << "File Data";
			SerializationData << YAML::BeginMap;

			{
				SerializationData << YAML::Key << "Package";
				SerializationData << YAML::Value << asset->GetFilePackage();
				SerializationData << YAML::Key << "Raw File";
				SerializationData << YAML::Value << asset->GetRawFilePath();
			}

			SerializationData << YAML::EndMap;
		}

		SerializationData << YAML::EndMap;
	}

	void FAssetSerializer::Deserialize(AAsset* asset, YAML::Node& deserializationData)
	{
		asset->Activate(deserializationData["Asset"].as<std::string>());
		asset->SetName(deserializationData["Name"].as<std::string>());
		asset->Category = deserializationData["Category"].as<std::string>();

		auto FileData = deserializationData["File Data"];
		asset->FilePackage = FileData["Package"].as<std::string>();
		asset->RawFilePath = FileData["Raw File"].as<std::string>();
		asset->RawFileExtension = asset->RawFilePath.extension().string();
	}

	void FAssetSerializer::Import(std::string filePath, AAsset* asset)
	{
		Deserialize(asset, Load(filePath));
		asset->SerializedFilePath = filePath;
		asset->IsSerialized = true;
	}

	void FAssetSerializer::Export(std::string filePath, AAsset* asset)
	{
		YAML::Emitter SerializationData;
		Serialize(asset, SerializationData);

		std::stringstream OutputStringStream;
		OutputStringStream << SerializationData.c_str();

		std::ofstream OutputFileStream(filePath);
		OutputFileStream << OutputStringStream.rdbuf();

		OutputFileStream.close();
	}

	YAML::Node FAssetSerializer::Load(std::string filePath)
	{
		std::ifstream InputFileStream(filePath);
		if (!InputFileStream.is_open())
		{
			SMessageHandler::Instance->SetFatal("Function",
				std::format("Failed to open asset serialization file : '{}'", filePath));
		}

		std::stringstream InputStringStream;
		InputStringStream << InputFileStream.rdbuf();

		std::string AssetFileString = InputStringStream.str();

		YAML::Node AssetData = YAML::Load(AssetFileString);

		return AssetData;
	}
}
