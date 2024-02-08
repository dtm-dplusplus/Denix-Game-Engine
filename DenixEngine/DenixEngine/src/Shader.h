#pragma once
#include <GL/glew.h>

class Shader
{
public:
	Shader();
	Shader(GLenum _type, std::string_view _src);
	GLuint CompileShader(GLenum _type, std::string_view _src);

private:
	GLuint m_ShaderID;
	std::string m_Source;

	friend class ShaderProgram;
};

class ShaderProgram
{
public:
	ShaderProgram();

	GLuint CreateProgram();

	void AttachShader(const Shader& _shader);

	bool LinkProgram() const;
	GLuint GetProgramID() const { return m_ShaderProgramID; }
private:
	GLuint m_ShaderProgramID;
	std::vector<Shader> m_Shaders;
};

