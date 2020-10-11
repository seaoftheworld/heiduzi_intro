#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // To use glm::lookUp()/perspective()/transform() ... apis
#include <glm/gtc/type_ptr.hpp>          // To use glm::make_mat4 in glm

#include <string>
#include <fstream>  // for reading shader file, texture file
#include <iostream>

// int tsInit(void);
// void tsDeInit(void);
// void tsDraw(void);

// void shaderTest(const std::string &shaderName);
// ...
// int  shaderTest_Init(const std::string &shaderName);
// void shaderTest_Deinit();
unsigned int load_shader(const std::string &shaderName);
void unload_shader(unsigned int prog);
void bind_shader(unsigned int prog);

int shader_update_alpha(unsigned int prog, float alpha);
int shader_update_mvp(unsigned int prog, const void *p);

// int  texTest_Init(const std::string &texName);
// void texTest_Deinit();
// void bindTex(GLenum textureUnit);

// int load_resources();
// void unload_resources();
int createTex(unsigned int num, unsigned int *texIds, std::string *imgPaths);
void destroyTex(unsigned int num, unsigned int *texIds);
// void bindTex(GLenum textureUnit, texEnum tex);
void bindTex(GLenum textureUnit, unsigned int texId);


// vert buff
// int vertBuffTest_Init();
// void vertBuffTest_Deinit();
// void vertBuffTest_Draw();

// struct vertAttr {
//     unsigned int vboId;
//     unsigned int offset;

//     unsigned int attrIdx;
//     unsigned int numFloat;
//     unsigned int stride;
// };
// void vertBuffAttr(unsigned int numAttr, vertAttr *attr);

void deinitVertBuffs(unsigned int num, unsigned int *vbo);
void   initVertBuffs(unsigned int num, unsigned int *vbo);

void deinitVertBuff(unsigned int *vbo);
void   initVertBuff(unsigned int *vbo);

int vertBuffData(unsigned int vbo, unsigned int size, const void *data);
void vertBuffAttrPtr(GLuint vbo, GLuint attrIdx, GLint attrSize, GLsizei attrStride, GLintptr vboOffset);



// Text, moved to api text.h/cpp
// struct Text {
//     // GLuint _vao = 0;
//     GLuint _vbo[2] = { 0 };

//     char* _text = NULL;
//     GLsizei _length = 0;
//     GLsizei vertexCount = 0;

//     // GLfloat* _vertices = NULL;
// };

// int init_text(Text* text);
// // int alloc_text(Text* text, const char* string);
// int alloc_text_buffers(Text* text);
// void deinit_text(Text* text);

// // void begin_draw_text(unsigned int progId, unsigned int texId);
// void draw_text(unsigned int prog, Text* text, const glm::mat4* projection2D_Mat, GLfloat x, GLfloat y, GLfloat scale);
