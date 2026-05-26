#include "GEngine/GShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace GShaderLoader {

    // Helper method to read a text file from your hard drive into a standard string
    std::string ReadShaderFile(const std::string& filePath) {
        std::ifstream shaderFile(filePath);
        if (!shaderFile.is_open()) {
            std::cerr << "CRITICAL: Could not open shader source file: " << filePath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << shaderFile.rdbuf();
        return buffer.str();
    }

    // Compiles a single shader type (Vertex or Fragment) and checks for syntax errors
    GLuint CompileShaderSource(GLenum shaderType, const std::string& sourceCode, const std::string& debugName) {
        GLuint shaderID = glCreateShader(shaderType);
        const char* rawSourceString = sourceCode.c_str();
        
        // Feed the text code array directly to the GPU driver memory slots
        glShaderSource(shaderID, 1, &rawSourceString, nullptr);
        glCompileShader(shaderID);

        // Check for errors in the GLSL code - it will not display an error to the terminal on its own
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> errorLog(logLength);
            glGetShaderInfoLog(shaderID, logLength, &logLength, errorLog.data());
            
            std::cerr << "GLSL COMPILATION ERROR in [" << debugName << "]:\n" << errorLog.data() << std::endl;
            
            glDeleteShader(shaderID);
            return 0;
        }
        return shaderID;
    }

    // The main execution hook called inside your MainWindow setup steps
    GLuint CreateShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
        // Read the shader text files
        std::string vertexCode = ReadShaderFile(vertexPath);
        std::string fragmentCode = ReadShaderFile(fragmentPath);

        // Compile shaders on the graphics card
        GLuint vertexShader = CompileShaderSource(GL_VERTEX_SHADER, vertexCode, "Vertex Shader");
        GLuint fragmentShader = CompileShaderSource(GL_FRAGMENT_SHADER, fragmentCode, "Fragment Shader");

        if (vertexShader == 0 || fragmentShader == 0) return 0;

        // Link the 2 shaders into a single graphics pipeline (GPU Program Pipeline)
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        // Verify that the output of one shader matches the input of the other
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> errorLog(logLength);
            glGetProgramInfoLog(programID, logLength, &logLength, errorLog.data());
            std::cerr << "SHADER LINKING ERROR:\n" << errorLog.data() << std::endl;
            glDeleteProgram(programID);
            return 0;
        }

        // Now that the pipeline is built, detach and delete the shader "building blocks"
        glDetachShader(programID, vertexShader);
        glDetachShader(programID, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return programID;
    }
}