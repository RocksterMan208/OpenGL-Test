#include "shaderclass.h"

std::string getFileContents(const char* fileName)
{
    std::ifstream in(fileName, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
}

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
    std::string vertexCode = getFileContents(vertexFilePath);
    std:: string fragmentCode = getFileContents(fragmentFilePath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Referencing then compiling the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    // Referencing then compiling the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    // Creating a shader program which will use the shaders and then apply them later on in the code.
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    // Now linking it to the shader program
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    // To keep things tidy, since the shaders are now implemented into OpenGL, they are no longer needed.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader,1024, NULL, infoLog);
            std::cout << "Shader compilation error for : " << type << "\n" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader linking error for : " << type << "\n" << std::endl;
        }
    }
}