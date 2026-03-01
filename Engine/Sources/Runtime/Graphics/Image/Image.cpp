#include <Core.h>
#include "Image.h"

namespace SE
{
	GImage::GImage()
	{
		this->ImageInstance = DirectX::ScratchImage();
	}

	GImage::GImage(unsigned int width, unsigned int height)
	{
		this->Initialize(width, height);
	}

	GImage::GImage(const glm::uvec2& size)
	{
		this->Initialize(size);
	}

	GImage::GImage(unsigned char* data, unsigned long long dataSize)
	{
		this->Initialize(data, dataSize);
	}

	GImage::GImage(const std::string& filePath)
	{
		this->Initialize(filePath);
	}

	GImage::~GImage()
	{
	}

	void GImage::Initialize(unsigned int width, unsigned int height)
	{
		SMessageHandler::Instance->Check(this->ImageInstance.Initialize2D(
			DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1));
	}

	void GImage::Initialize(const glm::uvec2& size)
	{
		this->Initialize(size.x, size.y);
	}

	void GImage::Initialize(unsigned char* data, unsigned long long dataSize)
	{
		SMessageHandler::Instance->Check(CoInitializeEx(null, COINIT_MULTITHREADED));

		SMessageHandler::Instance->Check(DirectX::LoadFromWICMemory(
			data, dataSize, DirectX::WIC_FLAGS_IGNORE_SRGB, null, this->ImageInstance));

		if (this->ImageInstance.GetMetadata().format != DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage ConvertedImage;
			SMessageHandler::Instance->Check(DirectX::Convert(*this->ImageInstance.GetImage(0, 0, 0),
				DXGI_FORMAT_R8G8B8A8_UNORM, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

			this->ImageInstance = std::move(ConvertedImage);
		}
	}

	void GImage::Initialize(const std::string& filePath)
	{
		SMessageHandler::Instance->Check(CoInitializeEx(null, COINIT_MULTITHREADED));

		SMessageHandler::Instance->Check(DirectX::LoadFromWICFile(std::wstring(filePath.begin(), filePath.end()).c_str(),
			DirectX::WIC_FLAGS_IGNORE_SRGB, null, this->ImageInstance));

		if (this->ImageInstance.GetMetadata().format != DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage ConvertedImage;
			SMessageHandler::Instance->Check(DirectX::Convert(*this->ImageInstance.GetImage(0, 0, 0),
				DXGI_FORMAT_R8G8B8A8_UNORM, DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, ConvertedImage));

			this->ImageInstance = std::move(ConvertedImage);
		}
	}

	void GImage::Clear(const GColor& clearColor)
	{
		auto& ImageData = *this->ImageInstance.GetImage(0, 0, 0);

		for (unsigned int y = 0; y < this->GetHeight(); y++)
		{
			auto RowStart = (GColor*)(ImageData.pixels + ImageData.rowPitch * y);
			std::fill(RowStart, RowStart + this->GetWidth(), clearColor);
		}
	}

	void GImage::Clear(const glm::uvec4& clearColor)
	{
		this->Clear(GColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
	}

	void GImage::ReplacePixel(unsigned int x, unsigned int y, GColor replaceColor)
	{
		auto& ImageData = *this->ImageInstance.GetImage(0, 0, 0);
		reinterpret_cast<GColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x] = replaceColor;
	}

	void GImage::ReplacePixel(const glm::uvec2& position, const glm::uvec4& replaceColor)
	{
		this->ReplacePixel(position.x, position.y, GColor(replaceColor.r, replaceColor.g, replaceColor.b, replaceColor.a));
	}

	void GImage::Export(const std::string& filePath)
	{
		const auto GetCodecId = [](const std::string& exportPath)
		{
			const std::filesystem::path filePath = exportPath;
			if (filePath.extension().string() == ".png")
			{
				return DirectX::WIC_CODEC_PNG;
			}
			else if (filePath.extension().string() == ".jpg")
			{
				return DirectX::WIC_CODEC_JPEG;
			}
			else if (filePath.extension().string() == ".bmp")
			{
				return DirectX::WIC_CODEC_BMP;
			}
			else
			{
				return (DirectX::WICCodecs)0;
			}
		};

		SMessageHandler::Instance->Check(DirectX::SaveToWICFile(*this->ImageInstance.GetImage(0, 0, 0),
			DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(GetCodecId(filePath)), std::wstring(filePath.begin(), filePath.end()).c_str()));
	}

	const GColor& GImage::GetPixel(unsigned int x, unsigned int y) const noexcept
	{
		auto& ImageData = *this->ImageInstance.GetImage(0, 0, 0);
		return reinterpret_cast<GColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x];
	}

	const GColor& GImage::GetPixel(const glm::uvec2& position) const noexcept
	{
		return this->GetPixel(position.x, position.y);
	}

	glm::uvec2 GImage::GetSize() const noexcept
	{
		return glm::uvec2(this->GetWidth(), this->GetHeight());
	}

	unsigned int GImage::GetWidth() const noexcept
	{
		return (unsigned int)this->ImageInstance.GetMetadata().width;
	}

	unsigned int GImage::GetHeight() const noexcept
	{
		return (unsigned int)this->ImageInstance.GetMetadata().height;
	}

	unsigned long long GImage::GetBytePitch() const noexcept
	{
		return (int)this->ImageInstance.GetImage(0, 0, 0)->rowPitch;
	}

	unsigned long long GImage::GetSlicePitch() const noexcept
	{
		return (int)this->ImageInstance.GetImage(0, 0, 0)->slicePitch;
	}

	GColor* GImage::GetData() const noexcept
	{
		return (GColor*)this->ImageInstance.GetPixels();
	}
}
