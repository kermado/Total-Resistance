#include <Engine/Texture.hpp>

#include <iostream>
#include <cstring>

#include <Engine/stb_image/stb_image.h>

namespace Engine
{
	Texture::Texture()
	: m_dirty(true)
	, m_width(0)
	, m_height(0)
	, m_repeat(false)
	, m_smooth(false)
	, m_pixels()
	, m_textureId(0)
	{
		// Generate OpenGL texture identifier.
		glGenTextures(1, &m_textureId);

		// Check for errors.
		assert(m_textureId > 0);
	}

	Texture::~Texture()
	{
		// Release the OpenGL texture object.
		glDeleteTextures(1, &m_textureId);
	}

	void Texture::Create(unsigned int width, unsigned int height)
	{
		assert(width > 0 && height > 0);

		// Clear the pixels vector of any existing data.
		m_pixels.clear();

		// Resize the pixels vector to be able to accomodate all of the data.
		m_pixels.resize(width * height * 4);

		// Save image dimensions.
		m_width = width;
		m_height = height;

		// Fill the pixels.
		for (unsigned int i = 0; i < width * height; ++i)
		{
			m_pixels[i * 4 + 0] = 255; // r
			m_pixels[i * 4 + 1] = 255; // g
			m_pixels[i * 4 + 2] = 255; // b
			m_pixels[i * 4 + 3] = 0; // a
		}

		// OpenGL texture object needs updating...
		m_dirty = true;
	}

	void Texture::Create(unsigned int width, unsigned int height, unsigned char* pixelData)
	{
		assert(width > 0 && height > 0);

		// Clear the pixels vector of any existing data.
		m_pixels.clear();

		// Resize the pixels vector to be able to accomodate all of the data.
		m_pixels.resize(width * height * 4);

		// Save image dimensions.
		m_width = width;
		m_height = height;

		// Copy the pixel data from the supplied array.
		// CAUTION: No bounds checking is performed!
		for (unsigned int i = 0; i < width * height * 4; ++i)
		{
			m_pixels[i] = pixelData[i];
		}

		// OpenGL texture object needs updating...
		m_dirty = true;
	}

	void Texture::Copy(const Texture& source, unsigned int x, unsigned int y)
	{
		assert(source.m_width > 0 && source.m_height > 0);
		assert(m_width > x + source.m_width && m_height > y + source.m_height);

		for (unsigned int row = 0; row < source.m_height; ++row)
		{
			for (unsigned int column = 0; column < source.m_width; ++column)
			{
				for (unsigned int channel = 0; channel < 4; ++channel)
				{
					m_pixels[((row + y) * m_width + (column + x)) * 4 + channel] = source.m_pixels[(row * source.m_width + column) * 4 + channel];
				}
			}
		}

		// OpenGL texture object needs updating...
		m_dirty = true;
	}

	bool Texture::LoadFromFile(std::string filepath)
	{
		// Clear the pixels vector of any existing data.
		m_pixels.clear();

		// Load the image.
		// Note: If STB_Image fails to load the image then the pointer returned
		// will be NULL and the width and height variables will be unchanged.
		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

		// OpenGL texture object needs updating...
		m_dirty = true;

		// Check that STB_Image was able to load the image.
		if (data && width > 0 && height > 0 && channels == 4)
		{
			// Check image dimensions.
			// Older graphics cards can only handle images that have dimensions
			// that are a power of 2.
			if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0)
			{
				std::cout << "WARNING: The image " << filepath
					<< " does not have dimensions that are a power of 2" << std::endl;
			}

			// Save image dimensions.
			m_width = static_cast<unsigned int>(width);
			m_height = static_cast<unsigned int>(height);

			// Resize the pixels vector to be able to accomodate all of the
			// data.
			m_pixels.resize(width * height * 4);

			// Copy the loaded data into the pixels vector.
			std::memcpy(&m_pixels[0], data, m_pixels.size());

			// Free the data loaded by STB_Image.
			// This is no longer needed since we have now populated our pixels
			// vector.
			stbi_image_free(data);
			data = nullptr;

			// Immediately pass the texture to OpenGL.
			// This is performed here so that the texture can be completely
			// loaded and ready for use in the loading thread.
			Update();

			// Success!
			return true;
		}
		else
		{
			std::cerr << "Failed to load image: " << filepath << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			return false;
		}
	}

	unsigned int Texture::GetWidth() const
	{
		return m_width;
	}

	unsigned int Texture::GetHeight() const
	{
		return m_height;
	}

	glm::vec2 Texture::GetDimensions() const
	{
		return glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height));
	}

	void Texture::SetRepeat(bool repeat)
	{
		m_repeat = repeat;
		m_dirty = true;
	}

	void Texture::SetSmooth(bool smooth)
	{
		m_smooth = smooth;
		m_dirty = true;
	}

	GLuint Texture::GetTextureId()
	{
		// Update the OpenGL texture object if necessary.
		if (m_dirty)
		{
			Update();
		}

		// Return the identifier for the OpenGL texture object.
		return m_textureId;
	}

	void Texture::Update()
	{
		// Bind the texture.
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		// Set texture properties.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);

		// Send the texture data.
		glTexImage2D(
			GL_TEXTURE_2D, // Target texture
			0, // Level-of-detail number
			GL_RGBA8, // Number of color components in the texture
			m_width, // Width of the texture image
			m_height, // Height of the texture image
			0, // Border (must be 0)
			GL_RGBA, // Format of the pixel data
			GL_UNSIGNED_BYTE, // Data type of the pixel data
			&m_pixels[0] // Pointer to the pixel data
		);

		// Unbind the texture so that it doesn't get accidentally modified from
		// the outside.
		glBindTexture(GL_TEXTURE_2D, 0);

		// Force an OpenGL flush so that the texture will be updated in all
		// contexts.
		glFlush();

		// Texture is no longer dirty.
		m_dirty = false;
	}
}
