#pragma once

#include "glApi/test.h"

#define TEXT_TEX_WIDTH  (256)
#define TEXT_TEX_HEIGHT (256)
#define TEXT_TEX_CHAR_WIDTH  (19)  // 13 chars per row: 13 * 19 == 247 (The last 9 pixels are empty)
#define TEXT_TEX_CHAR_HEIHGT (38)  //  6 chars per colomn: 38 * 6 == 228 (The last 28 pixels are empty)

// Common, also used for introstate
struct img_2d_dimension {  // used for introstate and text
    int w, h;
    float transX, transY, rotZ;
};

// Text
struct Text {
    // GLuint _vao = 0;
    GLuint _vbo[2] = { 0 };

    char* _text = NULL;
    GLsizei _length = 0;
    GLsizei vertexCount = 0;

    // GLfloat* _vertices = NULL;
};

int text_gen_vbos(Text *text);

// int alloc_text(Text *text, const char* string);
// int text_set_vbos(Text *text);
int text_set_char(Text* text, const char *c, unsigned int font_width, unsigned int font_height);

void text_del_vbos(Text *text);

// void begin_draw_text(unsigned int progId, unsigned int texId);
// void draw_text(unsigned int prog, Text *text, const glm::mat4* projection2D_Mat, GLfloat x, GLfloat y, GLfloat scale);

// Shader and texture should have already been bound before calling this.
// void draw_text(unsigned int prog, Text *text, const glm::mat4* projection2D_Mat, GLfloat x, GLfloat y, GLfloat scale);
void text_draw_vbos(Text *text);
