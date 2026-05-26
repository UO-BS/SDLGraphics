#version 330 core

// Input texture coordinates given by the vertex shader
in vec2 TexCoord; // Receives the mathematically interpolated (u,v) coordinate for this pixel

// The output pixel color
out vec4 FragColor; // The ultimate final color of this specific pixel (Red, Green, Blue, Alpha)

// Texture sampling
// This acts as a pipeline reference pointer to the texture image
uniform sampler2D textureSampler; 

void main() {
    // Outputs the pixel color sitting at the exact (u,v) coordinate location in the image map
    FragColor = texture(textureSampler, TexCoord);
}