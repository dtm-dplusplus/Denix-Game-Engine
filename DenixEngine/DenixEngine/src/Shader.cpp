#include "DePch.h"
#include "Shader.h"
#include "File.h"

Shader::Shader() : m_ShaderID{0}
{
}

Shader::Shader(const GLenum _type, const std::string_view _src)
{
    m_ShaderID = CompileShader(_type, _src);
}

GLuint Shader::CompileShader(const GLenum _type, const std::string_view _src)
{
    if (const GLuint shader = glCreateShader(_type))
    {
        // Create Shader
        m_ShaderID = shader;

        // Create Source
        m_Source = _src;
        const char* src = m_Source.c_str();
        glShaderSource(shader, 1, &src, NULL);

        // Compile Shader
        glCompileShader(shader);

        // Check for errors
        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            DE_LOG(LogShader, Error, "Shader Compilation Fail: %s", infoLog)
            return 0;
        }
        
        return m_ShaderID;
    }

    DE_LOG(Log, Error, "Failed to create shader\n")
    return 0;
}


ShaderProgram::ShaderProgram() : m_ShaderProgramID{ 0 }
{
}

GLuint ShaderProgram::CreateProgram()
{
    m_ShaderProgramID = glCreateProgram();

    if (!m_ShaderProgramID)
    {
        DE_LOG(Log, Error, "Failed to create shader program")
        return 0;
    }

    return m_ShaderProgramID;
}

void ShaderProgram::AttachShader(const Shader& _shader)
{
    glAttachShader(m_ShaderProgramID, _shader.m_ShaderID);
    m_Shaders.push_back(_shader);
}

bool ShaderProgram::LinkProgram() const
{
    glLinkProgram(m_ShaderProgramID);

    GLint result;
    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_ShaderProgramID, 512, NULL, infoLog);
        DE_LOG(Log, Error, "Shader Program Link Fail: %s", infoLog)
        return false;
    }

    // Detach and delete shaders
    for(const Shader& shader : m_Shaders)
    {
        glDetachShader(m_ShaderProgramID, shader.m_ShaderID);
        glDeleteShader(shader.m_ShaderID);
    }

    return true;
}
