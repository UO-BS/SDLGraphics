#ifndef G_SHADER_LOADER_H
#define G_SHADER_LOADER_H

#include <glad/glad.h>
#include <string>

namespace GShaderLoader {

    // Helper method to read a text file from your hard drive into a standard string
    std::string ReadShaderFile(const std::string& filePath);

    // Compiles a single shader type (Vertex or Fragment) and checks for syntax errors
    GLuint CompileShaderSource(GLenum shaderType, const std::string& sourceCode, const std::string& debugName);

    // The main execution hook called inside your MainWindow setup steps
    GLuint CreateShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
}


#endif