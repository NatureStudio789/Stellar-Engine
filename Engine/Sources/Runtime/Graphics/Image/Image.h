#ifndef _SE_IMAGE_H_
#define _SE_IMAGE_H_
#include "../../Core/MessageHandler/MessageHandler.h"
namespace SE
{
	class GColor
	{
	public:
		GColor() : ColorValue()
		{

		}
		GColor(unsigned int color)
		{
			this->ColorValue = color;
		}
		GColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			this->ColorValue = ((a << 24) | (r << 16) | (g << 8) | b);
		}
		GColor(unsigned char r, unsigned char g, unsigned char b) : GColor(r, g, b, 255)
		{

		}
		GColor(GColor color, unsigned char a) : GColor((a << 24) | color.ColorValue)
		{

		}
		GColor(const GColor& other)
		{
			this->ColorValue = other.ColorValue;
		}

		void SetA(unsigned char a)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (a << 24);
		}
		void SetR(unsigned char r)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (r << 16);
		}
		void SetG(unsigned char g)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (g << 8);
		}
		void SetB(unsigned char b)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | b;
		}

		GColor& operator=(GColor other)
		{
			this->ColorValue = other.ColorValue;
			return *this;
		}

		operator unsigned int()
		{
			return this->ColorValue;
		}

		const unsigned char GetA() const noexcept
		{
			return this->ColorValue >> 24;
		}
		const unsigned char GetR() const noexcept
		{
			return (this->ColorValue >> 16) & 0xFF;
		}
		const unsigned char GetG() const noexcept
		{
			return (this->ColorValue >> 8) & 0xFF;
		}
		const unsigned char GetB() const noexcept
		{
			return this->ColorValue & 0xFF;
		}

	public:
		unsigned int ColorValue;
	};

	class GImage
	{
	public:
		GImage();
		GImage(unsigned int width, unsigned int height);
		GImage(const glm::uvec2& size);
		GImage(unsigned char* data, unsigned long long dataSize);
		GImage(const std::string& filePath);
		GImage(GImage&& ) = default;
		GImage(const GImage& ) = delete;
		~GImage();

		void Initialize(unsigned int width, unsigned int height);
		void Initialize(const glm::uvec2& size);
		void Initialize(unsigned char* data, unsigned long long dataSize);
		void Initialize(const std::string& filePath);

		void Clear(const GColor& clearColor);
		void Clear(const glm::uvec4& clearColor);
		void ReplacePixel(unsigned int x, unsigned int y, GColor replaceColor);
		void ReplacePixel(const glm::uvec2& position, const glm::uvec4& replaceColor);
		void Export(const std::string& filePath);

		const GColor& GetPixel(unsigned int x, unsigned int y) const noexcept;
		const GColor& GetPixel(const glm::uvec2& position) const noexcept;
		glm::uvec2 GetSize() const noexcept;
		unsigned int GetWidth() const noexcept;
		unsigned int GetHeight() const noexcept;
		unsigned long long GetBytePitch() const noexcept;
		unsigned long long GetSlicePitch() const noexcept;
		GColor* GetData() const noexcept;

	private:
		DirectX::ScratchImage ImageInstance;
	};
}

#endif