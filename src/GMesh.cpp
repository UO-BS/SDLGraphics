#include <GEngine/GMesh.h>

GMesh::GMesh(const std::vector<GTextureVertex>& vertices, const GTexture* texture) {
    this->texture = texture;
    this->totalVertexCount = static_cast<GLsizei>(vertices.size());

    // Generate OpenGL handles
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GTextureVertex), vertices.data(), GL_STATIC_DRAW); 

    // Positions (x, y, z, w)
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GTextureVertex), (void*)0);
    glEnableVertexAttribArray(0);

    //  Texture UV Coordinates (u, v)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GTextureVertex), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GMesh::~GMesh() {
    // Cleanup VBO and VAO if they exist
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);

    // The GTexture pointer is handled by the manager
}

void GMesh::draw() const {
    if (VAO == 0 || texture == nullptr) return;

    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    glBindVertexArray(VAO);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, totalVertexCount);

    // Cleanup
    glBindVertexArray(0);
}