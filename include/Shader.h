#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

class Shader
{
public:
	Shader() : ID(0) {}

	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION
	};

	bool loadShaders(const char* vsFilePath, const char* fsFilePath, const char* gsFilePath = nullptr);
	void useProgram();
	GLuint getProgramID() const { return ID; }
    void validateProgram();
    void deleteShader();

	void setUniform(const GLchar* name, int x);
	void setUniformSamler(const GLchar* name, int slot);
	void setUniform(const GLchar* name, float x);
	void setUniform(const GLchar* name, bool cond);
	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, float x, float y);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, float x, float y, float z);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, float x, float y, float z, float w);
    void setUniform(const GLchar * name, const glm::mat3& m);
    void setUniform(const GLchar* name, const glm::mat4& m);
    GLint getUniformLocation(const GLchar* name);
private:
	std::string fileToString(const std::string& filePath);
    void checkCompileErrors(GLuint shader) const;
    void checkLinkErrors() const;
    void getAttribInfo() const;
    void getUniformInfo() const;
    GLuint ID;
	std::map<std::string, GLint> uniformLocation;
};

inline void Shader::deleteShader()
{
	glDeleteProgram(ID);
}

inline void Shader::setUniform(const GLchar * name, int x)
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, x);
}

inline void Shader::setUniform(const GLchar * name, float x)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, x);
}

inline void Shader::setUniform(const GLchar * name, bool cond)
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, cond);
}

inline void Shader::setUniform(const GLchar * name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2fv(loc, 1, &v[0]);
}

inline void Shader::setUniform(const GLchar * name, float x, float y)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, x, y);
}

inline void Shader::setUniform(const GLchar * name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3fv(loc, 1, &v[0]);
}

inline void Shader::setUniform(const GLchar * name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, x, y, z);
}

inline void Shader::setUniform(const GLchar * name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4fv(loc, 1, &v[0]);
}

inline void Shader::setUniform(const GLchar * name, float x, float y, float z, float w)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, x, y, z, w);
}

inline void Shader::setUniform(const GLchar * name, const glm::mat3& m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

inline void Shader::setUniform(const GLchar * name, const glm::mat4& m)
{
	GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

inline void Shader::getAttribInfo() const
{
    GLint maxLength, nAttribs;
    glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    GLchar* name = (GLchar*)malloc(maxLength);

    GLint written, size, location;
    GLenum type;
    printf("Index | Name\n");
    printf("----------------------------------------\n");
    for (int i=0; i < nAttribs; ++i) {
        glGetActiveAttrib(ID, i, maxLength, &written,
                          &size, &type, name);
        location = glGetAttribLocation(ID, name);
        printf("%-5d | %s\n", location, name);
    }

    free(name);
}

inline void Shader::getUniformInfo() const
{
    GLint maxLen, nUniforms;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &nUniforms);
    glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);

    GLchar* name = (GLchar*)malloc(maxLen);

    GLint written, size, location;
    GLenum type;
    printf("Index | Name\n");
    printf("----------------------------------------\n");
    for (int i=0; i < nUniforms; ++i) {
        glGetActiveUniform(ID, i, maxLen, &written,
                          &size, &type, name);
        location = glGetUniformLocation(ID, name);
        printf("%-5d | %s\n", location, name);
    }

    free(name);

}

inline void Shader::setUniformSamler(const GLchar * name, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, slot);
}
