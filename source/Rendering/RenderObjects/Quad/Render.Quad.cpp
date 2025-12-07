#include "Render.Quad.h"

void Quad::Init() {
    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f, // lu 0
        -1.0f, -1.0f, 0.0f, 0.0f, // ld 1
        1.0f, -1.0f, 1.0f, 0.0f, // rd 2
        1.0f,  1.0f, 1.0f, 1.0f  // ru 3
    };


    GLuint indices[] = {
        1, 0, 3,
        1, 3, 2
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Quad::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


Quad::Quad() {
    Init();
}

Quad::~Quad()
{
}
