#include <lit/rendering/pipeline_node.hpp>

using namespace lit::rendering;

void PipelineNodeAny::ResizeOutput(int width, int height) {
    if (m_output) {
        m_output->Resize(width, height);
    }
}

void PipelineNodeAny::Run() {
    Use();
    if (m_output) {
        m_output->Bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

float verticesRect[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
};

unsigned int indicesRect[] = {0, 1, 2, 3};

unsigned int VAO_Rect, VBO_Rect, EBO_Rect;

void lit::rendering::DrawQuad() {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        glGenVertexArrays(1, &VAO_Rect);
        glGenBuffers(1, &VBO_Rect);
        glGenBuffers(1, &EBO_Rect);

        glBindVertexArray(VAO_Rect);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_Rect);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRect), verticesRect, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Rect);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRect), indicesRect, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO_Rect);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
}

float verticesCube[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
};

unsigned int indicesCube[] = {
        0, 1, 3, 2,
        6, 7, 5, 4,
        0, 1, 5, 4,
        6, 7, 3, 2,
        0, 2, 6, 4,
        5, 7, 3, 1
};

unsigned int VAO_Cube, VBO_Cube, EBO_Cube;

void lit::rendering::DrawCube() {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        glGenVertexArrays(1, &VAO_Cube);
        glGenBuffers(1, &VBO_Cube);
        glGenBuffers(1, &EBO_Cube);

        glBindVertexArray(VAO_Cube);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Cube);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO_Cube);
    glDrawElements(GL_QUADS, 4 * 6, GL_UNSIGNED_INT, nullptr);
}
