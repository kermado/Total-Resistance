#include <Engine/Shader.hpp>

#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

namespace Engine
{
	Shader::Shader(Type shaderType)
	: m_id(0)
	, m_type(shaderType)
	{
		// Nothing to do.
	}

	Shader::~Shader()
	{
		if (m_id > 0)
		{
			// NOTE: The shader will continue to live on the graphics card if it
			// is being used by an active shader program.
			glDeleteShader(m_id);
		}
	}

	bool Shader::LoadFromFile(std::string filename)
	{
		std::ifstream file(filename.c_str(), std::ios::in);

		// Check that we were able to open the file.
		if (!file.is_open())
		{
			std::cerr << "ERROR: Unable to open shader file \"" << filename
				<< "\"." << std::endl;
			return false;
		}

		// String to hold the file contents.
		std::string fileContents;

		// String to hold each line of text.
		std::string line;

		// While we can still read a line...
		while (std::getline(file, line))
		{
			// Append the line to the file contents string.
			fileContents.append(line + "\n");
		}

		// Close the file stream.
		file.close();

		// Check that the file was not empty.
		if (fileContents.length() <= 0)
		{
			std::cerr << "ERROR: Shader file \"" << filename
				<< "\" was empty." << std::endl;
			return false;
		}

		// Create a new shader object of the specified type.
		m_id = glCreateShader(m_type);

		// Check that the shader object was created successfully.
		if (m_id <= 0)
		{
			std::cerr << "ERROR: Failed to create shader object for file \""
				<< filename << "\"." << std::endl;
			return false;
		}

		// Get the GLSL shader language version string.
		std::string glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
		glslVersion.erase(std::remove_if(glslVersion.begin(), glslVersion.end(), [](char c){ return (c == '.'); }), glslVersion.end());

		// Prepend the GLSL version to the shader source if it has not been already defined.
		if (fileContents.find("#version") != 0)
		{
			fileContents = "#version " + glslVersion + "\n" + fileContents;
		}

		// Get a character pointer to the shader source.
		const GLchar *shaderSource = fileContents.c_str();

		// Copy the source code to the shader object.
		glShaderSource(m_id, 1, &shaderSource, NULL);

		// Compile the shader.
		glCompileShader(m_id);

		// Verify the compilation status.
		GLint result;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			std::cerr << "ERROR: Shader \"" << filename
				<< "\" failed to compile." << std::endl;
			GLint logLength;
			glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				std::vector<char> compileError(logLength);
				glGetShaderInfoLog(m_id, logLength, NULL, &compileError[0]);
				std::cerr << &compileError[0] << std::endl;
			}

			// Compilation error.
			return false;
		}

		// We succeeded in loading and compiling the shader!
		return true;
	}

	Shader::Type Shader::GetShaderType() const
	{
		return m_type;
	}

	GLuint Shader::GetId() const
	{
		return m_id;
	}
}
