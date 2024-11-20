#pragma once
#include <string>

class Shader
{
private:
	std::string m_VFilePath;
	std::string m_FFilePath;
	unsigned int m_RendererId;
	//caching for uniforms
public:
	Shader(const std::string& vfilepath, const std::string& ffilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v3, float v4);
private:
	unsigned int CompileShader(unsigned int shaderType, const std::string& source);
	std::string ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};