// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 3.0 rendering.
#include "esUtil.h"

typedef struct {
    // Handle to a program object
    GLuint programObject;

} UserData;

//void testGetUniformBlock(GLint programObj) {
//    GLuint blockId, bufferId;
//    GLint blockSize;
//    GLuint bindingPoint = 1;
//    GLfloat lightData[] = {
//            1.0f, 0.0f, 0.0f, 0.0f,
//            0.0f, 0.0f, 0.0f, 1.0f
//    };
//
//    blockId = glGetUniformBlockIndex(programObj, "LightBlock");
//    glUniformBlockBinding(programObj, blockId, bindingPoint);
//    glGetActiveUniformBlockiv(programObj, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
//    glGenBuffers(1, &bufferId);
//    glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
//    glBufferData(GL_UNIFORM_BUFFER, blockSize, lightData, GL_DYNAMIC_DRAW);
//    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferId);
//}

void testGetActiveAttributes(GLint programObj) {
    GLint maxUniformLen = 0;
    GLint numUniforms = 0;
    char *uniformName = NULL;
    GLint index = 0;

    glGetProgramiv(programObj, GL_ACTIVE_ATTRIBUTES, &numUniforms);
    glGetProgramiv(programObj, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxUniformLen);

    uniformName = malloc(sizeof(char) * maxUniformLen);
    for (index = 0; index < numUniforms; index++) {
        GLint size = 0;
        GLenum type = 0;
        GLint location = 0;

        glGetActiveAttrib(programObj, index, maxUniformLen, NULL, &size, &type, uniformName);
        location = glGetAttribLocation(programObj, uniformName);
        esLogMessage("GetActiveAttributes: %s location:%d ", uniformName, location);
        switch (type) {
            case GL_FLOAT:
                esLogMessage("type:float\n");
                break;
            case GL_FLOAT_VEC2:
                esLogMessage("type:vec2\n");
                break;
            case GL_FLOAT_VEC3:
                esLogMessage("type:vec3\n");
                break;
            case GL_FLOAT_VEC4:
                esLogMessage("type:vec4\n");
                break;
            case GL_INT:
                esLogMessage("type:int\n");
                break;
            default:
                break;
        }
    }
}

void testGetActiveUniform(GLint programObj) {
    GLint maxUniformLen;
    GLint numUniforms;
    char *uniformName;
    GLint index;

    glGetProgramiv(programObj, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(programObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

    uniformName = malloc(sizeof(char) * maxUniformLen);
    for (index = 0; index < numUniforms; index++) {
        GLint size;
        GLenum type;
        GLint location;

        glGetActiveUniform(programObj, index, maxUniformLen, NULL, &size, &type, uniformName);
        location = glGetUniformLocation(programObj, uniformName);
        esLogMessage("GetActiveUniform: %s location:%d ", uniformName, location);
        switch (type) {
            case GL_FLOAT:
                esLogMessage("type:float\n");
                break;
            case GL_FLOAT_VEC2:
                esLogMessage("type:vec2\n");
                break;
            case GL_FLOAT_VEC3:
                esLogMessage("type:vec3\n");
                break;
            case GL_FLOAT_VEC4:
                esLogMessage("type:vec4\n");
                break;
            case GL_INT:
                esLogMessage("type:int\n");
                break;
            default:
                break;
        }
    }
}

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0) {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            esLogMessage("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

///
// Initialize the shader and program object
//
int Init(ESContext *esContext) {
    UserData *userData = esContext->userData;
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "layout (std140) uniform LightBlock       \n"
            "{                                        \n"
            "   vec3 lightDirection;                  \n"
            "   vec4 lightPosition;                   \n"
            "};                                       \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
            "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    // Create the program object
    programObject = glCreateProgram();

    if (programObject == 0) {
        return 0;
    }

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    testGetActiveAttributes(programObject);
    testGetActiveUniform(programObject);
    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            esLogMessage("Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return FALSE;
    }

    // Store the program object
    userData->programObject = programObject;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext) {
    UserData *userData = esContext->userData;
    GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f
    };

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObject);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Shutdown(ESContext *esContext) {
    UserData *userData = esContext->userData;

    glDeleteProgram(userData->programObject);
}

int esMain(ESContext *esContext) {
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB);

    if (!Init(esContext)) {
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
