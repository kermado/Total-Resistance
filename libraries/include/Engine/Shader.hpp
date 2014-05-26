#ifndef SHADER_H
#define	SHADER_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Engine/NonCopyable.hpp>

namespace Engine
{
	class Shader : private NonCopyable
	{
	public:
		/**
		 * Shader types.
		 */
		enum Type
		{
			VertexShader = GL_VERTEX_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER
		};

		/**
		 * Constructor.
		 *
		 * @param shaderType Type of shader.
		 */
		Shader(Type shaderType);

		/**
		 * Destructor.
		 */
		~Shader();

		/**
		 * Initializes the shader from the specified source code file and
		 * attempts to compile it.
		 *
		 * @param filename Path to the shader source code file.
		 * @return True if shader was initialized successfully.
		 */
		bool LoadFromFile(std::string filename);

		/**
		 * Returns the type of the shader.
		 *
		 * @return Shader type.
		 */
		Type GetShaderType() const;

		/**
		 * Returns the shader object identifier.
		 *
		 * @return Shader ID.
		 */
		GLuint GetId() const;

	private:
		/**
		 * Shader object identifier.
		 */
		GLuint m_id;

		/**
		 * Shader type.
		 */
		Type m_type;
	};
}

#endif
