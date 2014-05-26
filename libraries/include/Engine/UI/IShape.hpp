#ifndef ISHAPE_H
#define ISHAPE_H

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Engine/Texture.hpp>

namespace Engine
{
	namespace UI
	{
		class IShape
		{
		public:
			/**
			 * Destructor.
			 */
			virtual ~IShape();

			/**
			 * Returns the position of the shape.
			 *
			 * @return Position for the shape.
			 */
			glm::vec2 GetPosition() const;

			/**
			 * Sets the position of the shape.
			 *
			 * @param position Position for the shape.
			 */
			void SetPosition(const glm::vec2& position);

			/**
			 * Translates the shape by the specified vector.
			 *
			 * @param translation Translation vector.
			 */
			void Translate(const glm::vec2& translation);

			/**
			 * Returns the fill color for the shape (RGBA).
			 *
			 * @return Fill color.
			 */
			glm::vec4 GetFillColor() const;

			/**
			 * Sets the fill color for the shape.
			 *
			 * @param fillColor Fill color (RGBA).
			 */
			void SetFillColor(const glm::vec4& fillColor);

			/**
			 * Returns a shared pointer to the texture that should be applied
			 * to the shape.
			 *
			 * @return Shared pointer to the texture.
			 */
			std::shared_ptr<Texture> GetTexture() const;

			/**
			 * Sets the texture that should be applied to the shape.
			 *
			 * @param texture Shared pointer to the texture.
			 */
			void SetTexture(std::shared_ptr<Texture> texture);

			/**
			 * Returns true if the shape contains the specified point.
			 *
			 * @note Evaluating this method is quite expensive. Derived classes
			 * should override this method to use a less expensive algorithm
			 * if possible.
			 *
			 * @param point Viewport coordinate to check.
			 * @return True if the shape contains the point.
			 */
			virtual bool ContainsPoint(const glm::vec2& point);

			/**
			 * Returns the number of vertices that compose the shape.
			 *
			 * @return Number of vertices that compose the shape.
			 */
			virtual unsigned int GetVertexCount() const = 0;

			/**
			 * Returns the transformation matrix for the shape.
			 *
			 * @return Transformation matrix.
			 */
			glm::mat4 GetTransformationMatrix();

			/**
			 * Returns the Vertex Array Object identifier for the shape.
			 *
			 * @return Vertex Array Object identifier.
			 */
			GLuint GetVAO();

		protected:
			/**
			 * Default constructor.
			 */
			IShape();

			/**
			 * Specifies that the OpenGL buffers need updating.
			 */
			void ShouldUpdate();

			/**
			 * Transforms the specified point by applying the shape's
			 * transformation matrix.
			 *
			 * @param point Point to transform
			 * @return Transformed point.
			 */
			glm::vec2 TransformPoint(const glm::vec2& point);

			/**
			 * Returns the position of the vertex specified by the supplied
			 * index.
			 *
			 * @param index Index for the vertex position to retrieve. This must
			 * be in the range [0, GetVertexCount() - 1].
			 * @return The position of the index-th vertex.
			 */
			virtual glm::vec2 GetVertexPosition(unsigned int index) const = 0;

			/**
			 * Returns the UV texture coordinates for the vertex specified by
			 * the provided index.
			 *
			 * @param index Vertex index for the UV coordinate to return.
			 * @return UV texture coordinate for the specified vertex index.
			 */
			virtual glm::vec2 GetTextureCoordinate(unsigned int index) const;

		private:
			/**
			 * Updates the OpenGL buffers.
			 *
			 * @note Check the dirty flag to determine whether it is necessary
			 * to call this method.
			 */
			void Update();

		private:
			/**
			 * True if the OpenGL buffers need to be updated.
			 */
			bool m_dirty;

			/**
			 * Position of the shape.
			 */
			glm::vec3 m_position;

			/**
			 * Fill color for the shape.
			 */
			glm::vec4 m_fillColor;

			/**
			 * Shared pointer to the texture that should be applied
			 * to the shape.
			 */
			std::shared_ptr<Texture> m_texture;

			/**
			 * Vertex Array Object identifier.
			 */
			GLuint m_VAO;

			/**
			 * Identifier for the vertex positions buffer object.
			 */
			GLuint m_positionVBO;

			/**
			 * Identifier for the vertex color buffer object.
			 */
			GLuint m_colorVBO;

			/**
			 * Identifier for the vertex texture coordinates buffer object.
			 */
			GLuint m_textureCoordinatesVBO;
		};
	}
}

#endif
