#include "esUtil.h"
#include <memory>
#include <iosfwd>

typedef struct {
    GLuint programObject;
    GLuint vboIds[2];
    GLuint offsetLoc;
} UserData;

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

const char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec4 a_color;      \n"
        "uniform float u_offset;                    \n"
        "out vec4 v_color;                          \n"
        "void main()                                \n"
        "{                                          \n"
        "    v_color = a_color;                     \n"
        "    gl_Position = a_position;              \n"
        "    gl_Position.x += u_offset;             \n"
        "}";


const char fShaderStr[] =
        "#version 300 es            \n"
        "precision mediump float;   \n"
        "in vec4 v_color;           \n"
        "out vec4 o_fragColor;      \n"
        "void main()                \n"
        "{                          \n"
        "    o_fragColor = v_color; \n"
        "}";

int Init(ESContext *esContext) {
    auto userData = static_cast<UserData *>(esContext->userData);
    GLuint programObject = esLoadProgram(vShaderStr, fShaderStr);
    userData->offsetLoc = glGetUniformLocation(programObject, "u_offset");

    if (programObject == 0) {
        return GL_FALSE;
    }

    userData->programObject = programObject;
    userData->vboIds[0] = 0;
    userData->vboIds[1] = 0;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

void DrawPrimitiveWithoutVBOs(GLfloat *vertices,
                              GLint vtxStride,
                              GLint numIndices,
                              GLushort *indices) {
    GLfloat *vtxBuf = vertices;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE,
                          GL_FLOAT, GL_FALSE, vtxStride, vtxBuf);
    vtxBuf += VERTEX_POS_SIZE;
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE,
                          GL_FLOAT, GL_FALSE, vtxStride, vtxBuf);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);
}

void DrawPrimitiveWithVBOs(ESContext *esContext,
                           GLint numVertices, GLfloat *vtxBuf,
                           GLint vtxStride, GLint numIndices,
                           GLushort *indices) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    GLuint offset = 0;
    if (userData->vboIds[0] == 0 && userData->vboIds[1] == 0) {
        glGenBuffers(2, userData->vboIds);
        glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, vtxBuf, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices,
                     GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);


//    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE,
//                          GL_FLOAT, GL_FALSE, vtxStride, static_cast<const void *>(offset));
//    offset += VERTEX_POS_SIZE * sizeof(GLfloat);
//    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE,
//                          GL_FLOAT, GL_FALSE, vtxStride, static_cast<const void *>(offset));

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Draw(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);

    // 3 vertices, with (x,y,z) ,(r, g, b, a) per-vertex
    GLfloat vertices[3 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] =
            {
                    -0.5f, 0.5f, 0.0f,        // v0
                    1.0f, 0.0f, 0.0f, 1.0f,  // c0
                    -1.0f, -0.5f, 0.0f,        // v1
                    0.0f, 1.0f, 0.0f, 1.0f,  // c1
                    0.0f, -0.5f, 0.0f,        // v2
                    0.0f, 0.0f, 1.0f, 1.0f,  // c2
            };

    // Index buffer data
    GLushort indices[3] = {0, 1, 2};

    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData->programObject);

    glUniform1f(userData->offsetLoc, 0.0f);
    DrawPrimitiveWithoutVBOs(vertices, sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE), 3,
                             indices);

    glUniform1f(userData->offsetLoc, 1.0f);
    DrawPrimitiveWithVBOs(esContext, 3, vertices,
                          sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE), 3, indices);
}

void Shutdown(ESContext *esContext) {
    auto userData = static_cast<UserData *>(esContext->userData);
    glDeleteProgram(userData->programObject);
    glDeleteBuffers(2, userData->vboIds);
}

int esMain(ESContext *esContext) {
    esContext->userData = malloc(sizeof(UserData));
    esCreateWindow(esContext, "VertexBufferObjects", 320, 240, ES_WINDOW_RGB);

    if (!Init(esContext)) {
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);
    return GL_TRUE;
}

