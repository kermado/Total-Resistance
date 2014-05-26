#include <Engine/ShaderProgram.hpp>

#include <iostream>
#include <vector>
#include <cassert>

namespace Engine
{
	ShaderProgram::ShaderProgram()
	: m_id(0)
	, m_uniformLocationCache()
	{
		// Create the shader program object.
		m_id = glCreateProgram();

		// Check that the program object was created successfully.
		assert(m_id > 0);
	}

	ShaderProgram::~ShaderProgram()
	{
		// NOTE: The program will continue to live on the graphics card until
		// it is no longer in use by any rendering context.
		glDeleteProgram(m_id);
	}

	void ShaderProgram::AttachShader(const Shader& shader)
	{
		glAttachShader(m_id, shader.GetId());
	}

	bool ShaderProgram::Link()
	{
		// Clear uniform location cache.
		m_uniformLocationCache.clear();

		// Specify default attribute locations.
		// This must be set before linking the shader program.
		glBindAttribLocation(m_id, 0, "v_vertPosition");
		glBindAttribLocation(m_id, 1, "v_vertNormal");
		glBindAttribLocation(m_id, 2, "v_vertColor");
		glBindAttribLocation(m_id, 3, "v_vertTextureCoordinates");

		// Try to link the shader program.
		glLinkProgram(m_id);

		// Verify that the program was linked successfully.
		GLint result;
		glGetProgramiv(m_id, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			std::cerr << "ERROR: Failed to link shader program" << std::endl;
			GLint logLength;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				std::vector<char> linkError(logLength);
				glGetProgramInfoLog(m_id, logLength, NULL, &linkError[0]);
				std::cerr << &linkError[0] << std::endl;
			}

			return false;
		}

		return true;
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::SetUniformMatrix4fv(std::string name, const glm::mat4& matrix)
	{
		GLint uniformLocation = GetUniformLocation(name);
		if (uniformLocation >= 0)
		{
			glUniformMatrix4fv(
				uniformLocation,
				1,
				GL_FALSE,
				&matrix[0][0]
			);
		}
	}

	void ShaderProgram::SetUniformMatrix3fv(std::string name, const glm::mat3& matrix)
	{
		GLint uniformLocation = GetUniformLocation(name);
		if (uniformLocation >= 0)
		{
			glUniformMatrix3fv(
				uniformLocation,
				1,
				GL_FALSE,
				&matrix[0][0]
			);
		}
	}

	void ShaderProgram::SetUniform3fv(std::string name, const glm::vec3& vector)
	{
		GLint uniformLocation = GetUniformLocation(name);
		if (uniformLocation >= 0)
		{
			glUniform3fv(
				uniformLocation,
				1,
				&vector[0]
			);
		}
	}

	void ShaderProgram::SetUniform1f(std::string name, float value)
	{
		GLint uniformLocation = GetUniformLocation(name);
		if (uniformLocation >= 0)
		{
			glUniform1f(
				uniformLocation,
				value
			);
		}
	}

	void ShaderProgram::SetUniform1i(std::string name, int value)
	{
		GLint uniformLocation = GetUniformLocation(name);
		if (uniformLocation >= 0)
		{
			glUniform1i(
				uniformLocation,
				value
			);
		}
	}

	GLint ShaderProgram::GetUniformLocation(std::string name)
	{
		auto iter = m_uniformLocationCache.find(name);
		if (iter != m_uniformLocationCache.end())
		{
			return iter->second;
		}
		else
		{
			const GLint location = glGetUniformLocation(m_id, name.c_str());
			m_uniformLocationCache[name] = location;
			return location;
		}
	}

	GLuint ShaderProgram::GetId() const
	{
		return m_id;
	}
}
