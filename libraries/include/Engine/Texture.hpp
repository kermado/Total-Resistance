#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Engine
{
	class Texture
	{
	public:
		/**
		 * Default constructor.
		 */
		Texture();

		/**
		 * Destructor.
		 */
		~Texture();

		/**
		 * Initializes an empty texture of the specified dimensions and with
		 * all pixels being white transparent.
		 *
		 * @param width Width for the texture.
		 * @param height Height for the texture.
		 */
		void Create(unsigned int width, unsigned int height);

		/**
		 * Initializes a texture of the specified dimensions and with the pixels
		 * coppied from the supplied pixel data array.
		 *
		 * @note Pixel data should be supplied in row-major form and of the
		 * format [r, g, b, a, r, g, b, a, ...] .
		 *
		 * @param width Width for the texture.
		 * @param height Height for the texture.
		 */
		void Create(unsigned int width, unsigned int height, unsigned char* pixelData);

		/**
		 * Pastes the contents of the source texture at the specified
		 * offset coordinates.
		 *
		 * @param source The source texture to copy from.
		 * @param x The x-coordinate offset to start pasting the source texture.
		 * @param y The y-coordinate offset to start pasting the source texture.
		 */
		void Copy(const Texture& source, unsigned int x, unsigned int y);

		/**
		 * Loads the texture from an image file.
		 *
		 * @param filepath Path to the image file.
		 * @return True if the texture was loaded successfully.
		 */
		bool LoadFromFile(std::string filepath);

		/**
		 * Returns the texture's width.
		 *
		 * @return Texture width (in pixels).
		 */
		unsigned int GetWidth() const;

		/**
		 * Returns the texture's height.
		 *
		 * @return Texture height (in pixels).
		 */
		unsigned int GetHeight() const;

		/**
		 * Returns the texture's dimensions.
		 *
		 * @return Texture dimensions.
		 */
		glm::vec2 GetDimensions() const;

		/**
		 * Sets whether the texture should be repeated (tiled).
		 *
		 * @param repeat True if the texture should be repeated.
		 */
		void SetRepeat(bool repeat);

		/**
		 * Sets whether the texture should be smoothed.
		 *
		 * @param smooth True if the texture should be smoothed.
		 */
		void SetSmooth(bool smooth);

		/**
		 * Returns the OpenGL texture object identifier.
		 *
		 * @return Texture object identifier.
		 */
		GLuint GetTextureId();

	private:
		/**
		 * Updates the OpenGL texture object.
		 */
		void Update();

	private:
		/**
		 * True if the OpenGL texture object needs to be updated.
		 */
		bool m_dirty;

		/**
		 * Texture width (in pixels).
		 */
		unsigned int m_width;

		/**
		 * Texture height (in pixels).
		 */
		unsigned int m_height;

		/**
		 * Should the texture be repeated (titled).
		 */
		bool m_repeat;

		/**
		 * Should the texture be smoothed (less pixelated).
		 */
		bool m_smooth;

		/**
		 * Pixels that compose the texture.
		 */
		std::vector<unsigned char> m_pixels;

		/**
		 * OpenGL texture object ID.
		 */
		GLuint m_textureId;
	};
}

#endif
