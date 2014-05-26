#ifndef SHADERPROGRAM_H
#define	SHADERPROGRAM_H

#include <unordered_map>

#include <glm/glm.hpp>

#include <Engine/NonCopyable.hpp>
#include <Engine/Shader.hpp>

namespace Engine
{
	class ShaderProgram : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 */
		ShaderProgram();

		/**
		 * Destructor.
		 */
		virtual ~ShaderProgram();

		/**
		 * Attaches the specified shader to the program.
		 *
		 * @param shader Reference to the shader to attach.
		 */
		void AttachShader(const Shader& shader);

		/**
		 * Links the shader program.
		 *
		 * @return True if the program was linked successfully.
		 */
		bool Link();

		/**
		 * Use the shader program.
		 */
		void Use() const;

		/**
		 * Sets the uniform specified by the provided name with the 4x4 matrix
		 * of floating point values.
		 *
		 * @param name Name for the uniform to set.
		 * @param matrix Matrix value to assign to the uniform.
		 */
		void SetUniformMatrix4fv(std::string name, const glm::mat4& matrix);

		/**
		 * Sets the uniform specified by the provided name with the 3x3 matrix
		 * of floating point values.
		 *
		 * @param name Name for the uniform to set.
		 * @param matrix Matrix value to assign to the uniform.
		 */
		void SetUniformMatrix3fv(std::string name, const glm::mat3& matrix);

		/**
		 * Sets the uniform specified by the provided name with the 3-dimensional
		 * vector of floating point values.
		 *
		 * @param name Name for the uniform to set.
		 * @param matrix Vector value to assign to the uniform.
		 */
		void SetUniform3fv(std::string name, const glm::vec3& vector);

		/**
		 * Sets the uniform specified by the provided name with the floating
		 * point value.
		 *
		 * @param name Name for the uniform to set.
		 * @param matrix Floating point value to assign to the uniform.
		 */
		void SetUniform1f(std::string name, float value);

		/**
		 * Sets the uniform specified by the provided name with the integer
		 * value.
		 *
		 * @param name Name for the uniform to set.
		 * @param matrix Integer value to assign to the uniform.
		 */
		void SetUniform1i(std::string name, int value);

		/**
		 * Returns the location of the uniform specified by the provided name.
		 *
		 * @param name Name for the uniform.
		 * @return Uniform location.
		 */
		GLint GetUniformLocation(std::string name);

		/**
		 * Returns the OpenGL identifier for the shader program.
		 *
		 * @return OpenGL object identifier.
		 */
		GLuint GetId() const;

	private:
		/**
		 * Program identifier.
		 */
		GLuint m_id;

		/**
		 * Cache for uniform locations.
		 */
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
	};
}

#endif
