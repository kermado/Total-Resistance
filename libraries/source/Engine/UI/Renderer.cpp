#include <Engine/UI/Renderer.hpp>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Attribute/ShaderProgram.hpp>

namespace Engine
{
	namespace UI
	{
		Renderer::Renderer(std::shared_ptr<Window> window,
			std::shared_ptr<ResourceManager> resourceManager)
		: m_window(window)
		, m_resourceManager(resourceManager)
		{
			// Nothing to do.
		}

		Renderer::~Renderer()
		{
			// Nothing to do.
		}

		void Renderer::RenderShape(UI::IShape& shape,
			std::shared_ptr<ShaderProgram> shaderProgram)
		{
			// We can only render with a valid shader program.
			if (shaderProgram)
			{
				// Get the window dimensions;
				const unsigned int viewportWidth = m_window->GetWidth();
				const unsigned int viewportHeight = m_window->GetHeight();

				// Calculate the projection matrix.
				const glm::mat4 projectionMatrix = glm::ortho<float>(
					0.0f, // Left
					static_cast<float>(viewportWidth), // Right
					static_cast<float>(viewportHeight), // Bottom
					0.0f, // Top
					-1.0f, // Near
					1.0f // Far
				);

				// Get the model matrix.
				const glm::mat4 modelMatrix = shape.GetTransformationMatrix();

				// Compute the Model-View-Projection (MVP) matrix.
				const glm::mat4 mvpMatrix = projectionMatrix * modelMatrix;

				// Start using the shader program.
				shaderProgram->Use();

				// Get the loaction of the Model-View-Projection (MVP) matrix
				// uniform in the shader.
				const GLint mvpMatrixLocation = glGetUniformLocation(
					shaderProgram->GetId(),
					"modelViewProjectionMatrix"
				);
				assert(mvpMatrixLocation >= 0);

				// Pass the MVP matrix to the shader.
				glUniformMatrix4fv(
					mvpMatrixLocation,
					1,
					GL_FALSE,
					&mvpMatrix[0][0]
				);

				// Get the shape's texture.
				std::shared_ptr<Texture> texture = shape.GetTexture();

				// Get the location of the texture flag uniform in the shader.
				const GLint textureFlagLocation = glGetUniformLocation(
					shaderProgram->GetId(),
					"useTexture"
				);
				assert(textureFlagLocation >= 0);

				// Set the texture flag value.
				glUniform1i(textureFlagLocation, texture ? 1 : 0);

				// Pass the texture to the shader if the shape has a texture.
				if (texture)
				{
					// Get the location of the texture uniform in the shader.
					const GLint textureLocation = glGetUniformLocation(
						shaderProgram->GetId(),
						"textureUnit"
					);
					assert(textureLocation >= 0);

					// Activate a texture unit.
					glActiveTexture(GL_TEXTURE0 + 0);

					// Bind the shape's texture to the texture unit.
					glBindTexture(GL_TEXTURE_2D, shape.GetTexture()->GetTextureId());

					// Pass the texture unit to the shader attribute.
					glUniform1i(textureLocation, 0);
				}

				// Bind the shape's VAO.
				GLuint shapeVAO = shape.GetVAO();
				assert(shapeVAO > 0);
				glBindVertexArray(shapeVAO);

				// Draw the shape.
				glDrawArrays(GL_TRIANGLES, 0, shape.GetVertexCount());

				// Unbind the shape's VAO.
				glBindVertexArray(0);

				// Stop using the shader.
				glUseProgram(0);
			}
		}
	}
}
