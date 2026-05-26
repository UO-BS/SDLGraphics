#version 330 core

// Receive data from VBO
layout (location = 0) in vec4 aPos; // Receives your 4D world position (x, y, z, w)
layout (location = 1) in vec2 aTex; // Receives your 2D texture coordinate (u, v)

// Create output variable
out vec2 TexCoord; // Passes the texture mapping coordinate down the pipeline

// Model Matrix and Projection Matrix passed in by OpenGL function
uniform mat4 modelMatrix;    // Holds your object's specific position matrix
uniform mat4 viewProjMatrix; // Holds your camera's combined view & projection matrix

void main() {
    // Math operations happen from right to left:
    // Take the raw local 3D vertex position, move it into world space via modelMatrix,
    // and then apply the global camera perspective lens using viewProjMatrix.
    gl_Position = viewProjMatrix * modelMatrix * aPos;

    // Simply pass the texture coordinate along to the pixel system completely unaltered
    TexCoord = aTex;
}