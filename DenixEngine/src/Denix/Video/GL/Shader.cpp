#include "Shader.h"

#include "Denix/Core/FileSubsystem.h"

GLuint Denix::Shader::CreateProgram()
{
    if (const GLuint program = glCreateProgram())
    {
        m_GL_ID = program;
        return program;
    }

    DE_LOG(LogShader, Error, "Failed to create shader program")
    return 0;
}

void Denix::Shader::DeleteProgram() const
{
    if (m_GL_ID)
    {
        glDeleteProgram(m_GL_ID);
    }
}

bool Denix::Shader::LinkProgram() const
{
    glLinkProgram(m_GL_ID);

    GLint result;
    glGetProgramiv(m_GL_ID, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_GL_ID, 1024, NULL, infoLog);
        DE_LOG(LogShader, Error, "GLShader Program Link Fail: {}", infoLog)
        return false;
    }

    // DE_LOG(LogShader, Trace, "GLShader Program Link Success")

    return true;
}

bool Denix::Shader::CompileShader(ShaderSource& _sourceObj) const
{
    // Check Shader Source
    if (_sourceObj.Source.empty())
    {
        DE_LOG(LogShader, Error, "Failed to read shader source: {}", _sourceObj.Path)
        return false;
    }
    // Get Shader Type
    if (const GLenum type = GetShaderType(_sourceObj.Source); type != GL_FALSE)
    {
        _sourceObj.Type = type;
    }
    else
    {
        DE_LOG(LogShader, Error, "Failed to get shader type")
        return false;
    }

    // Compile Shader
    if (const GLuint shader = glCreateShader(_sourceObj.Type))
    {
        const char* src = _sourceObj.Source.c_str();
        glShaderSource(shader, 1, &src, NULL);

        glCompileShader(shader);

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            DE_LOG(LogShader, Error, "GLShader Error: {}", infoLog)
            return false;
        }

        glAttachShader(m_GL_ID, shader);
        glDeleteShader(shader);
        return true;
    }

    DE_LOG(LogShader, Error, "Failed to create shader\n")
    return false;
}

GLenum Denix::Shader::GetShaderType(const std::string& _source) const
{
    if (const auto keywordIt = _source.find_first_of(g_SHADER_KEYWORD))
    {
        std::string type;

        for (auto typeIt = keywordIt + g_SHADER_KEYWORD_OFFSET;
             _source[typeIt] != ' ' && _source[typeIt] != '\n'; typeIt++)
        {
            type += _source[typeIt];
        }

        // Return the type if we find it
        for (const auto& [StringType, EnumType] : g_SHADER_TYPES)
            if (type == StringType) return EnumType;

    }

    DE_LOG(LogShader, Error, "Shader Keyword DE_SHADER not found or invalid type")

    return GL_FALSE;
}

bool Denix::Shader::CompileProgram()
{
    if (!m_GL_ID)
    {
        DE_LOG(LogShader, Error, "Invalid shader program ID: {}", GetDirectoryName())
        return false;
    }
			
    if (m_ShaderSources.empty())
    {
        DE_LOG(LogShader, Error, "No shader sources to compile: {}", GetDirectoryName())
        return false;
    }
			
    for (ShaderSource& source : m_ShaderSources)
    {
        // Account for recompliation. If we have a source being edited in memory, we don't need to read it from disk
        if (source.Source.empty())
        {
            source.Source = FileSubsystem::ReadFile(source.Path);

            // Check our source after reading
            if (source.Source.empty())
            {
                DE_LOG(LogShader, Error, "Failed to read shader source: {}", source.Path)
                return false;
            }
        }
				
        if (!CompileShader(source)) return false;
    }

    if (!LinkProgram())
    {
        DeleteProgram();
        return false;
    }
    return true;
}

GLint Denix::Shader::GetUniform(const std::string& _uniform)
{
    // Cache Uniform Result
    if (!m_ShaderUniforms.contains(_uniform))
    {
        if (const GLint uniform = glGetUniformLocation(m_GL_ID, _uniform.c_str()); uniform != -1)
        {
            m_ShaderUniforms[_uniform] = uniform;
            return uniform;
        }
				
        DE_LOG(LogShader, Error, "Failed to get uniform: {}", _uniform)
        return -1;
    }

    return  m_ShaderUniforms[_uniform];
}
