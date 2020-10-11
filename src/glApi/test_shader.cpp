#include "test.h"

// GLuint _vsh, _fsh, _prog;

// static inline void deinitShader(GLuint *shaderId) {
//     if (shaderId) {
//         glDeleteShader(*shaderId);
//         *shaderId = 0;
//     }
// }

int initShader(std::string path, GLuint *id, GLenum type) {
    std::ifstream filePath;
    std::string source = "", line;
    GLuint shaderId;

    // Read Vertex Shader source code from the file and
    // create vsh-id if successs.
    filePath.open(path);
    if (!filePath.is_open()) {
        printf("Fail to find shader file: %s\n", path.c_str());
        return 0;
    }
    while (std::getline(filePath, line)) {
        source += line + "\n";
    }
    filePath.close();

    shaderId = glCreateShader(type);
    if (!shaderId) {
        printf("glCreateShader(): Failed to generate shader id for \'%s\' file!\n", path.c_str());
        return 0;
    }

    if (id) {
        *id = shaderId;
    }

    // Compile the Shader and check the result
    const GLchar* sh_source = source.c_str();
    glShaderSource(shaderId, 1, &sh_source, NULL);

    GLint Result;
    glCompileShader(shaderId); 
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
    if (!Result) {
        std::string log;
        GLint       logSize;

        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize);
        log.reserve(logSize);
        glGetShaderInfoLog(shaderId, logSize, NULL, (char*)log.data());

        printf("shader Error:\n%s\n", log.c_str());

        if (id) {
            // deinitShader(id);
            glDeleteShader(*id);
        }
        else {
            // deinitShader(&shaderId);
            glDeleteShader(shaderId);
        }

        return 0;
    }

    return 1;
}

int initProg(GLuint vsh, GLuint fsh, GLuint *prog) {
    if (!prog) {
        return 0;
    }

    *prog = glCreateProgram();
    if(*prog == 0) {
        printf("glCreateProgram(): failed to create program !\n");
        return 0;
    }

    // Link and make sure the progrma linked correctly
    glAttachShader(*prog, vsh);
    glAttachShader(*prog, fsh);
    glLinkProgram(*prog);

    GLint linkStatus;
    glGetProgramiv(*prog, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        std::string log;
        GLint       logSize;

        glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &logSize);
        log.resize(logSize);
        log.reserve(logSize);
        glGetProgramInfoLog(*prog, logSize, NULL, (char *)log.data());

        glDeleteProgram(*prog);
        
        printf("glLinkProgram() Failed: \n%s\n", log.c_str());
        return 0;
    }

    printf("  gl prog generated, id: \'%d'\n", *prog);

    glDetachShader(*prog, vsh);
    glDetachShader(*prog, fsh);
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    return 1;
}

// void deinitProg(GLuint *prog) {
//     if (!prog) {
//         return;
//     }

//     glDeleteProgram(*prog);
//     printf("  gl prog: %d deleted.\n", *prog);
//     *prog = 0;
// }
void deinitProg(GLuint prog) {
    glDeleteProgram(prog);
    printf("  gl prog: %d deleted.\n", prog);
}

void setShaderToUse(GLuint prog) {
    glUseProgram(prog);
}

void resetShaderToUse() {
    glUseProgram(0);
}

GLint blendAlpha_Uniform1f(GLuint prog, GLfloat alpha) {
    const std::string &uniformName = "blendAlpha";
    // const std::string &uniformName = "blendAlphaafdsafsd";

    GLint loc = glGetUniformLocation(prog, uniformName.c_str());
    if (loc == -1) {
        printf("Shader unable to find uniform: %s!\n", uniformName.c_str());
        return loc;
    }

    glUniform1f(loc, alpha);
    return loc;
}

GLint mvp_UniformMatrix4fv(GLuint prog, const void *p) {
    if (!p) {
        return 0;
    }

    const std::string &uniformName = "mvp";
    GLint loc = glGetUniformLocation(prog, uniformName.c_str());
    if (loc == -1) {
        printf("Shader unable to find uniform: %s!\n", uniformName.c_str());
        return loc;
    }

    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)p);
    return loc;
}

int shader_update_alpha(unsigned int prog, float alpha) {
    return (int)blendAlpha_Uniform1f(prog, (GLfloat)alpha);
}

int shader_update_mvp(unsigned int prog, const void *p) {
    return (int)mvp_UniformMatrix4fv(prog, p);
}

/*
void shaderTest(const std::string &shaderName) {
    GLuint vsh, fsh, prog;

    std::string vshPath = "./data/shaders/" + shaderName + ".vsh";
    std::string fshPath = "./data/shaders/" + shaderName + ".fsh";

    if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
        return;
    }

    if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
        return;
    }

    if (!initProg(vsh, fsh, &prog)) {
        return;
    }

    {
        setShaderToUse(prog);

        GLfloat alpha = 0.8f;
        GLint result = blendAlpha_Uniform1f(prog, alpha);
        if (result == -1) {
            printf("Failed to set \'blendAlpha\', -1 result returned.\n");
        }
        else {
            printf("blendAlpha's location is %d, set tobe 0.8f.\n", result);
        }

        alpha = 0.4f;
        result = blendAlpha_Uniform1f(prog, alpha);
        if (result == -1) {
            printf("Failed to set \'blendAlpha\', -1 result returned.\n");
        }
        else {
            printf("blendAlpha's location is %d, set tobe 0.4f.\n", result);   
        }

        resetShaderToUse();
    }

    deinitShader(&vsh);
    deinitShader(&fsh);
    deinitProg(&prog);
}

int shaderTest_Init(const std::string &shaderName) {
    GLuint vsh, fsh, prog;

    std::string vshPath = "./data/shaders/" + shaderName + ".vsh";
    std::string fshPath = "./data/shaders/" + shaderName + ".fsh";

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
        return 0;
    }

    if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
        return 0;
    }

    if (!initProg(vsh, fsh, &prog)) {
        return 0;
    }

    _vsh = vsh; 
    _fsh = fsh; 
    _prog = prog;
    
    setShaderToUse(prog);
    // GLint loc = blendAlpha_Uniform1f(prog, 0.2f);
    // printf("loc returned from blendAlpha_Uniform1f() is: %d\n", loc);

    // GLfloat read;
    // glGetUniformfv(prog, loc, &read);
    // printf("alpha read from shader is: %f\n", read);

    return 1;
}

// void shaderTest_Deinit() {
//     deinitShader(&_vsh);
//     deinitShader(&_fsh);
//     deinitProg(&_prog);
// }

void shaderTest_Deinit(unsigned int prog) {
    deinitProg(prog);
}
//*/

unsigned int load_shader(const std::string &shaderName) {
    
    GLuint vsh, fsh, prog;

    std::string vshPath = "./data/shaders/" + shaderName + ".vsh";
    std::string fshPath = "./data/shaders/" + shaderName + ".fsh";


    if (!initShader(vshPath, &vsh, GL_VERTEX_SHADER)) {
        return 0;
    }

    if (!initShader(fshPath, &fsh, GL_FRAGMENT_SHADER)) {
        return 0;
    }

    if (!initProg(vsh, fsh, &prog)) {
        return 0;
    }

    // _vsh = vsh; 
    // _fsh = fsh; 
    // _prog = prog;
    // return 1;

    return prog;
}

// void unload_shader() {
//     deinitShader(&_vsh);
//     deinitShader(&_fsh);
//     deinitProg(&_prog);
// }
void unload_shader(unsigned int prog) {
    deinitProg(prog);
}

// Only 1 shader is used now, so no input parameter is requried.
// void bind_shader() {
//     setShaderToUse(_prog);
// }
void bind_shader(unsigned int prog) {
    setShaderToUse(prog);
}
