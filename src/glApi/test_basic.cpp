#include "test.h"

GLbyte vShaderStr[] =
"attribute vec4 vPosition;    \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = vPosition;  \n"
"}                            \n";

GLbyte fShaderStr[] =
"precision mediump float;\n"\
"void main()                                  \n"
"{                                            \n"
"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
"}                                            \n";

GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
                       -0.5f, -0.5f, 0.0f,
                        0.5f, -0.5f, 0.0f };
GLuint programObject;

// Create a shader object, load the shader source, and
// compile the shader.
// GLuint tsLoadShader(GLenum type, const char* shaderSrc)
static GLuint tsLoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader = 0;
    GLint compiled = 0;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
        return 0;

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
            // char* infoLog = malloc(sizeof(char) * infoLen);
            // glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            // esLogMessage("Error compiling shader:\n%s\n", infoLog);
            // free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Initialize the shader and program object
int tsInit(void)
{
    // GLuint vertexShader;
    // GLuint fragmentShader;
    // GLuint prgObject;
    // GLint linked;

    if (gl3wInit()) {
        return 0;
    }
    std::cout << "  gl3wInit() complete! OpenGL info:" << std::endl;
    std::cout << "    Vendor:  " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "    Render:  " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;

    /*
    // Load the vertex/fragment shaders
    vertexShader = tsLoadShader(GL_VERTEX_SHADER, (char *)vShaderStr);
    fragmentShader = tsLoadShader(GL_FRAGMENT_SHADER, (char *)fShaderStr);
    if (!vertexShader || !fragmentShader) {
        return 0;
    }

    // Create the program object
    prgObject = glCreateProgram();
    if (!prgObject)
        return 0;

    // Attach shader and Bind vPosition in shader to attribute 0   
    glAttachShader(prgObject, vertexShader);
    glAttachShader(prgObject, fragmentShader);
    glBindAttribLocation(prgObject, 0, "vPosition");

    // Link the program and Check the link status
    glLinkProgram(prgObject);
    glGetProgramiv(prgObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(prgObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            // char* infoLog = malloc(sizeof(char) * infoLen);
            // glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            // esLogMessage("Error linking program:\n%s\n", infoLog);
            // free(infoLog);
        }

        glDeleteProgram(prgObject);
        return 0;
    }

    // Store the program object
    programObject = prgObject;

    glDetachShader(prgObject, vertexShader);
    glDetachShader(prgObject, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //*/

    // glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return 1;
}

void tsDeInit(void)
{
    glDeleteProgram(programObject);
}

// Draw a triangle using the shader pair created in Init()
void tsDraw(void)
{
    // Set the viewport
    //  the origin, width, and height of the 2D rendering surface that will be drawn to
    // glViewport(0, 0, esContext->width, esContext->height);
    // glViewport(0, 0, 800, 400);

    // Clear the and set the program object to be used
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programObject);

    // Load the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 
        3, GL_FLOAT, 
        GL_FALSE, 
        3 * sizeof(GL_FLOAT), 
        vVertices);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glDisableVertexAttribArray(0);
}
