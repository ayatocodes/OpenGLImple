#include "Shader.hpp"

#include "Renderer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const std::string& vfilepath, const std::string& ffilepath) : m_VFilePath(vfilepath), m_FFilePath(ffilepath)
{
    GLCALL(std::string vShader = ParseShader(m_VFilePath));
    std::string fShader = ParseShader(m_FFilePath);

    GLCALL(m_RendererId = CreateShader(vShader, fShader));
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

std::string Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss[1];

    while (getline(stream, line))
    {
        ss[0] << line << '\n';
    }

    return ss[0].str();
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& source)
{
    unsigned int shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int shaderCompileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompileStatus);

    //Checking if the shader has compiled successfully 
    if (shaderCompileStatus != GL_TRUE)
    {
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* message = (char*)malloc(logLength * sizeof(char));
        glGetShaderInfoLog(shader, logLength, &logLength, message);
        std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << "\n";
        std::cout << message << "\n";
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCALL(unsigned int program = glCreateProgram());

    GLCALL(unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCALL(unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCALL(glAttachShader(program, vShader));
    GLCALL(glAttachShader(program, fShader));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    GLCALL(glDeleteShader(vShader));
    GLCALL(glDeleteShader(fShader));

    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    GLCALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));
    if (location == -1) std::cout << "error\n";
    return location;
}
