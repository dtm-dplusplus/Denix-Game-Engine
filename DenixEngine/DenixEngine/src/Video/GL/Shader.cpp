#include "DePch.h"
#include "Shader.h"
#include "File.h"


std::unique_ptr<Shader> ShaderProgram::CompileShader(const GLenum _type, const std::string_view _src)
{
    if (std::unique_ptr<Shader> shader = std::make_unique<Shader>(_type, _src); shader->GetID())
    {
        // Create Source
        const char* src = shader->m_Source.data();
        glShaderSource(shader->m_glID, 1, &src, NULL);

        // Compile Shader
        glCompileShader(shader->m_glID);

        // Check for errors
        GLint result;
        glGetShaderiv(shader->m_glID, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader->m_glID, 512, NULL, infoLog);
            DE_LOG(LogShader, Error, "Shader Compilation Fail: {}", infoLog)
            return nullptr;
        }

        DE_LOG(LogShader, Trace, "Shader Compilation Success: {}", shader->m_glID)
        m_Shaders.push_back(std::move(shader));
        return shader;
    }

    DE_LOG(Log, Error, "Failed to create shader\n")
    return nullptr;
}



GLuint ShaderProgram::CreateProgram()
{
    if (const GLuint program = glCreateProgram())
    {
        m_glID = program;
        DE_LOG(LogShader, Trace, "Created shader program ID: {}", program)

		return program;
    }

    DE_LOG(Log, Error, "Failed to create shader program")
    return 0;
}

void ShaderProgram::AttachShaders() const
{
    for (const auto& shader : m_Shaders) AttachShader(shader->m_glID);
}

void ShaderProgram::DetachShaders() const
{
    for (const std::unique_ptr<Shader>& shader : m_Shaders)
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
        DE_LOG(Log, Error, "Shader Program Link Fail: {}", infoLog)
        return false;
    }

    DetachShaders();

    return true;
}
