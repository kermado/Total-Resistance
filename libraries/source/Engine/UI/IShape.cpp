#include <Engine/UI/IShape.hpp>

#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	namespace UI
	{
		IShape::IShape()
		: m_dirty(true) // Buffers need updating
		, m_position(glm::vec3(0.0f, 0.0f, 0.0f)) // Origin
		, m_fillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) // Solid white
		, m_texture(nullptr)
		, m_VAO(0)
		, m_positionVBO(0)
		, m_colorVBO(0)
		, m_textureCoordinatesVBO(0)
		{
			// Generate Vertex Array Object.
			glGenVertexArrays(1, &m_VAO);

			// Generate buffers.
			glGenBuffers(1, &m_positionVBO);
			glGenBuffers(1, &m_colorVBO);
			glGenBuffers(1, &m_textureCoordinatesVBO);

			// Check for errors.
			assert(m_VAO > 0);
			assert(m_positionVBO > 0);
			assert(m_colorVBO > 0);
			assert(m_textureCoordinatesVBO > 0);
		}

		IShape::~IShape()
		{
			// Release VAO.
			if (m_VAO > 0)
			{
				glDeleteVertexArrays(1, &m_VAO);
				m_VAO = 0;
			}

			// Release position VBO.
			if (m_positionVBO > 0)
			{
				glDeleteBuffers(1, &m_positionVBO);
				m_positionVBO = 0;
			}

			// Release color VBO.
			if (m_colorVBO > 0)
			{
				glDeleteBuffers(1, &m_colorVBO);
				m_colorVBO = 0;
			}

			// Release texture coordinate VBO.
			if (m_textureCoordinatesVBO > 0)
			{
				glDeleteBuffers(1, &m_textureCoordinatesVBO);
				m_textureCoordinatesVBO = 0;
			}
		}

		glm::vec2 IShape::GetPosition() const
		{
			return glm::vec2(m_position.x, m_position.y);
		}

		void IShape::SetPosition(const glm::vec2& position)
		{
			m_position = glm::vec3(position, 0.0f);
			m_dirty = true;
		}

		void IShape::Translate(const glm::vec2& translation)
		{
			m_position += glm::vec3(translation , 0.0f);
			m_dirty = true;
		}

		glm::vec4 IShape::GetFillColor() const
		{
			return m_fillColor;
		}

		void IShape::SetFillColor(const glm::vec4& fillColor)
		{
			assert(fillColor.x >= 0.0f && fillColor.x <= 1.0f);
			assert(fillColor.y >= 0.0f && fillColor.y <= 1.0f);
			assert(fillColor.z >= 0.0f && fillColor.z <= 1.0f);
			assert(fillColor.w >= 0.0f && fillColor.w <= 1.0f);

			m_fillColor = fillColor;
			m_dirty = true;
		}

		std::shared_ptr<Texture> IShape::GetTexture() const
		{
			return m_texture;
		}

		void IShape::SetTexture(std::shared_ptr<Texture> texture)
		{
			m_texture = texture;
			m_dirty = true;
		}

		bool IShape::ContainsPoint(const glm::vec2& point)
		{
			// For each triangle.
			for (unsigned int t = 0; t < GetVertexCount() / 3; ++t)
			{
				// Index for the first vertex that composes triangle.
				const unsigned int triangleStartIndex = t * 3;

				// Get the viewport coordinates of the three vertices that
				// compose the triangle.
				glm::vec2 A = TransformPoint(GetVertexPosition(triangleStartIndex));
				glm::vec2 B = TransformPoint(GetVertexPosition(triangleStartIndex + 1));
				glm::vec2 C = TransformPoint(GetVertexPosition(triangleStartIndex + 2));

				// Compute barycentric coordinates for the specified point with
				// respect to the triangle ABC.
				const glm::vec2 v0(C - A);
				const glm::vec2 v1(B - A);
				const glm::vec2 v2(point - A);
				float dot00 = glm::dot(v0, v0);
				float dot01 = glm::dot(v0, v1);
				float dot02 = glm::dot(v0, v2);
				float dot11 = glm::dot(v1, v1);
				float dot12 = glm::dot(v1, v2);
				float recipDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
				float u = (dot11 * dot02 - dot01 * dot12) * recipDenom;
				float v = (dot00 * dot12 - dot01 * dot02) * recipDenom;

				// Return true if the point is inside of the triangle.
				if (u >= 0.0f && v >= 0.0f && (u + v) < 1.0f) return true;
			}

			// The point was not contained by any of the triangles that compose
			// the shape.
			return false;
		}

		glm::mat4 IShape::GetTransformationMatrix()
		{
			return glm::translate(m_position);
		}

		GLuint IShape::GetVAO()
		{
			if (m_dirty)
			{
				// Update the buffers.
				Update();

				// Shape is now clean.
				m_dirty = false;
			}

			return m_VAO;
		}

		void IShape::ShouldUpdate()
		{
			m_dirty = true;
		}

		glm::vec2 IShape::TransformPoint(const glm::vec2& point)
		{
			glm::vec4 homogeneousPoint(point, 0.0f, 1.0f);
			homogeneousPoint = GetTransformationMatrix() * homogeneousPoint;
			return glm::vec2(homogeneousPoint);
		}

		glm::vec2 IShape::GetTextureCoordinate(unsigned int index) const
		{
			// We can only return valid texture coordinates if the shape
			// has a texture.
			if (m_texture)
			{
				// Get the position of the vertex position.
				const glm::vec2 vertexPosition = GetVertexPosition(index);

				// Get the texture dimensions.
				const glm::vec2 textureDimensions(
					static_cast<float>(m_texture->GetWidth()),
					static_cast<float>(m_texture->GetHeight())
				);

				// Return the vertex position normalised by the texture
				// dimensions.
				return vertexPosition / textureDimensions;
			}
			else
			{
				return glm::vec2(0.0f, 0.0f);
			}
		}

		void IShape::Update()
		{
			const unsigned int numVertices = GetVertexCount();

			// Get all the data.
			std::unique_ptr<glm::vec3[]> vertexPositions(new glm::vec3[numVertices]);
			std::unique_ptr<glm::vec4[]> vertexColors(new glm::vec4[numVertices]);
			std::unique_ptr<glm::vec2[]> vertexTextureCoordinates(new glm::vec2[numVertices]);
			for (unsigned int i = 0; i < numVertices; ++i)
			{
				const glm::vec2 position = GetVertexPosition(i);
				vertexPositions[i] = glm::vec3(position, 0.0f);
				vertexColors[i] = m_fillColor;
				vertexTextureCoordinates[i] = GetTextureCoordinate(i);
			}

			// Bind the Vertex Array Object.
			glBindVertexArray(m_VAO);

			// Send the vertex position data to the corresponding VBO.
			glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(glm::vec3) * numVertices,
				&vertexPositions[0],
				GL_STATIC_DRAW
			);

			// Set the vertex position data to be the first attribute
			// passed to the shader (index 0).
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Send the vertex color data to the corresponding VBO.
			glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(glm::vec4) * numVertices,
				&vertexColors[0],
				GL_STATIC_DRAW
			);

			// Set the vertex color data to be the third attribute
			// passed to the shader (index 2).
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

			// Texturing...
			if (m_texture)
			{
				// Send the texture coordinate data to the corresponding VBO.
				glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinatesVBO);
				glBufferData(
					GL_ARRAY_BUFFER,
					sizeof(glm::vec2) * numVertices,
					&vertexTextureCoordinates[0],
					GL_STATIC_DRAW
				);

				// Set the vertex texture coordinate data to be the fourth
				// attribute passed to the shader (index 3).
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}

			// Unbind the Vertex Array Object.
			glBindVertexArray(0);
		}
	}
}
