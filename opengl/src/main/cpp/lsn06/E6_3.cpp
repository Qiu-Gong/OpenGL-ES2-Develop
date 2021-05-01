#include "esUtil.h"
#include <memory>
#include <iosfwd>

/**
 * 顶点数组
 */
typedef struct {
    GLuint programObject;
} UserData;

const char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 1) in vec4 a_position;   \n"
        "layout(location = 0) in vec4 a_color;      \n"
        "out vec4 v_color;                          \n"
        "void main()                                \n"
        "{                                          \n"
        "    v_color = a_color;                     \n"
        "    gl_Position = a_position;              \n"
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

const GLfloat color[4] = {
        1.0f, 0.0f, 0.0f, 1.0f
};

const GLfloat vertexPos[3 * 3] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
};

int Init(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    auto program = esLoadProgram(vShaderStr, fShaderStr);
    if (program == 0) {
        return GL_FALSE;
    }
    userData->programObject = program;
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

void Draw(ESContext *esContext) {
    auto userData = static_cast<UserData *>(esContext->userData);

    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData->programObject);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vertexPos);
    glEnableVertexAttribArray(1);
    glVertexAttrib4fv(0, color);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(1);
}

void Shutdown(ESContext *esContext) {
    auto *userData = static_cast<UserData *>(esContext->userData);
    glDeleteProgram(userData->programObject);
}

extern "C" {
int esMain(ESContext *esContext) {
    esContext->userData = malloc(sizeof(UserData));
    esCreateWindow(esContext, "E6_3", 320, 240, ES_WINDOW_RGB);
    if (!Init(esContext)) {
        return GL_FALSE;
    }
    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);
    return GL_TRUE;
}
}