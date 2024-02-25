#include "DePch.h"
#include "Shader.h"

GLuint ShaderProgram::CreateProgram()
{
    if (const GLuint program = glCreateProgram())
    {
        m_glID = program;
        DE_LOG(LogShader, Trace, "Created shader program ID: {}", program)

		return program;
    }

    DE_LOG(LogShader, Error, "Failed to create shader program")
    return 0;
}

void ShaderProgram::AttachShaders() const
{
    for (const auto& shader : m_Shaders) AttachShader(shader->m_glID);
}

void ShaderProgram::DetachShaders() const
{
    for (const std::shared_ptr<Shader>& shader : m_Shaders)
    {
        DetachShader(shader->m_glID);
        shader->Delete();

        // Destroy shaders in later builds. Keeping now for debugging
        // m_Shaders.clear();
    }
}

bool ShaderProgram::LinkProgram() const
{
    glLinkProgram(m_glID);

    GLint result;
    glGetProgramiv(m_glID, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_glID, 512, NULL, infoLog);
        DE_LOG(LogShader, Error, "Shader Program Link Fail: {}", infoLog)
        return false;
    }

    DetachShaders();

    return true;
}