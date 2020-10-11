#include "text.h"

#define TEXT2D_POSITION_ATTR_IDX 0
#define TEXT2D_TEXCOORD_ATTR_IDX 1

#define TEXT2D_POSITION_ATTR_SIZE 3
#define TEXT2D_TEXCOORD_ATTR_SIZE 2
#define TEXT2D_VERTEX_ATTR_SIZE (TEXT2D_POSITION_ATTR_SIZE + TEXT2D_TEXCOORD_ATTR_SIZE)



float uvWidth = ((float)TEXT_TEX_CHAR_WIDTH / TEXT_TEX_WIDTH);
float uvHeight = ((float)TEXT_TEX_CHAR_HEIHGT / TEXT_TEX_HEIGHT);

static void calculate_6_rect_vert_pos(float* rect_vert_3_6_float, unsigned int expectedWidth, unsigned int expectedHeight) {
    if (!rect_vert_3_6_float) {
        return;
    }

    // -320.0f,  240.0f, 0.0f,  // left  top      -
    // -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
    //  320.0f, -240.0f, 0.0f,  // right bottom   |_\

    //  320.0f, -240.0f, 0.0f,  // right bottom   __
    //  320.0f,  240.0f, 0.0f,  // right top      \ |
    // -320.0f,  240.0f, 0.0f,  // left  top       \|
    if (expectedWidth > 0 && expectedHeight > 0) {
        for (unsigned int i = 0; i < (3 * 6); i += 3) {
            rect_vert_3_6_float[i + 0] = (float)(expectedWidth / 2.0f);
            rect_vert_3_6_float[i + 1] = (float)(expectedHeight / 2.0f);
            rect_vert_3_6_float[i + 2] = 0.0f;
        }

        unsigned int invertIdx[] = {
            0,
            (1 * 3 + 0), (1 * 3 + 1),
                         (2 * 3 + 1),
                         (3 * 3 + 1),
            (5 * 3 + 0),
        };
        for (unsigned int i = 0; i < sizeof(invertIdx) / sizeof(unsigned int); i++) {
            rect_vert_3_6_float[invertIdx[i]] = 0.0f - rect_vert_3_6_float[invertIdx[i]];
        }
    }
}

static void calculate_6_rect_vert_uv(float* vert_uv, unsigned int row, unsigned int col, float uvWidth, float uvHeight) {
    if (!vert_uv) {
        return;
    }

    printf("  uv info: row-col: %d-%d, uv-w-h: %f-%f\n", row, col, uvWidth, uvHeight);
    
    // float uv_buffer[] = {
    //     0.0f, 0.0f,  // left  top        -   
    //     0.0f, 1.0f,  // left  bottom     |\ 
    //     1.0f, 1.0f,  // right bottom     |_\

    //     1.0f, 1.0f,  // right  bottom  __
    //     1.0f, 0.0f,  // right  top     \ |
    //     0.0f, 0.0f,  // left   top      \|
    // };
    float uv_buffer[6 * 2] = {
        (float)col * uvWidth, (float)row * uvHeight,                // left  top     -   
        (float)col * uvWidth, (float)(row + 1) * uvHeight,          // left  bottom  |\ 
        (float)(col + 1) * uvWidth, (float)(row + 1) * uvHeight,    // right bottom  |_\

        (float)(col + 1) * uvWidth, (float)(row + 1) * uvHeight,    // right  bottom  __
        (float)(col + 1) * uvWidth, (float)row * uvHeight,          // right  top     \ |
        (float)col * uvWidth, (float)row * uvHeight                 // left   top      \|
    };

    if (uvWidth > 0 && uvHeight > 0) {
        // printf("  vert_uv[]:\n");
        for (unsigned int i = 0; i < (6 * 2); i++) {
            vert_uv[i] = uv_buffer[i];
            // if (!(i % 2)) {
            //     printf("\n");
            // }
            // printf("  %d: %f,", i, vert_uv[i]);

        }
        // printf("\n\n");
    }

}

int text_gen_vbos(Text* text)
{
    // Text *text = (Text*)calloc(1, sizeof(Text));
    if (!text) {
        printf("  init_text(): input parameter is NULL !\n");
        return 0;
    }

    // glGenVertexArrays(1, &text->_vao);
    glGenBuffers(2, text->_vbo);

    // _GLT_ASSERT(text->_vao);
    // _GLT_ASSERT(text->_vbo);
    // if (!text->_vao || !text->_vbo)
    // {
    //     gltDeleteText(text);
    //     return GLT_NULL;
    // }
    // if (!text->_vao || !text->_vbo[0] || !text->_vbo[1]) {
    if (!text->_vbo[0] || !text->_vbo[1]) {
        printf("  init_text(): generating vao/vbo failed !\n");
        return 0;
    }

    // glBindVertexArray(text->_vao);

    // glBindBuffer(GL_ARRAY_BUFFER, text->_vbo[0]);
    // glEnableVertexAttribArray(_GLT_TEXT2D_POSITION_LOCATION);
    // glVertexAttribPointer(_GLT_TEXT2D_POSITION_LOCATION, _GLT_TEXT2D_POSITION_SIZE, GL_FLOAT, GL_FALSE, (_GLT_TEXT2D_VERTEX_SIZE * sizeof(GLfloat)), (const void*)(_GLT_TEXT2D_POSITION_OFFSET * sizeof(GLfloat)));
    // glEnableVertexAttribArray(_GLT_TEXT2D_TEXCOORD_LOCATION);
    // glVertexAttribPointer(_GLT_TEXT2D_TEXCOORD_LOCATION, _GLT_TEXT2D_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, (_GLT_TEXT2D_VERTEX_SIZE * sizeof(GLfloat)), (const void*)(_GLT_TEXT2D_TEXCOORD_OFFSET * sizeof(GLfloat)));

    // vertBuffAttrPtr(text->_vbo[0], TEXT2D_POSITION_ATTR_IDX, TEXT2D_POSITION_ATTR_SIZE, TEXT2D_POSITION_ATTR_SIZE * sizeof(float), 0);  // pos attr
    // vertBuffAttrPtr(text->_vbo[1], TEXT2D_TEXCOORD_ATTR_IDX, TEXT2D_TEXCOORD_ATTR_SIZE, TEXT2D_TEXCOORD_ATTR_SIZE * sizeof(float), 0);  // uv  attr

    // glBindVertexArray(0);

    return 1;
}

static void free_text_ptr_00(Text* text)
{
    if (!text)
        return;

    if (text->_text) {
        free(text->_text);
        text->_text = NULL;
    }

    // if (text->_vertices) {
    //     free(text->_vertices);
    //     text->_vertices = NULL;
    // }

    text->_length = 0;
}

static void free_text_ptr_01(Text* text)
{
    if (!text)
        return;

    // if (text->_vertices) {
    //     free(text->_vertices);
    //     text->_vertices = NULL;
    // }

    text->vertexCount = 0;
}

void text_del_vbos(Text* text)
{
    if (!text)
        return;

    // if (text->_vao) {
    //     glDeleteVertexArrays(1, &text->_vao);
    //     text->_vao = 0;
    // }

    if (text->_vbo[0]) {
        glDeleteBuffers(1, &text->_vbo[0]);
        text->_vbo[0] = 0;
    }
    if (text->_vbo[1]) {
        glDeleteBuffers(1, &text->_vbo[1]);
        text->_vbo[1] = 0;
    }

    // if (text->_text) {
    //     // text->_text = NULL;  // if text is statically allocated
    //     free(text->_text);      // if text is dynamically allocated (read in from data)

    //     text->_text = NULL;
    //     text->_textLength = 0;
    // }
    // if (text->_vertices) {
    //     // text->_vertices = NULL;
    //     free(text->_vertices);

    //     text->_vertices = NULL;
    //     text->vertexCount = 0;
    // }
    free_text_ptr_00(text);
    free_text_ptr_01(text);

    // free(text);
}

// static GLint char_count(const char *str)
// {
//     if (!str)
//         return 0;

//     GLint count = 0;

//     while ((*str) != '\0') {
//         // if (gltIsCharacterDrawable(*str))
//         //     count++;

//         count++;
//         str++;
//     }

//     return count;
// }

int alloc_text(Text* text, const char* string)
{
    if (!text || !string) {
        printf("  alloc_text(): failed to alloc since input is NULL !\n");
        return 0;
    }

    size_t strLength = strlen(string);
    if (!strLength) {
        printf("  alloc_text(): failed to alloc since strLen is 0 !\n");
        return 0;
    }

    // if (text->_text) {
    //     if (strcmp(string, text->_text) == 0) {
    //         return GL_TRUE;
    //     }
    // }
    // free(text->_text);
    // text->_text = NULL;

    free_text_ptr_00(text);
    free_text_ptr_01(text);
    text->_text = (char*)malloc((strLength + 1) * sizeof(char));

    if (text->_text) {
        memcpy(text->_text, string, (strLength + 1) * sizeof(char));
        text->_length = strLength;
        // text->_dirty = GL_TRUE;

        // text->vertexCount = char_count(string);
        text->vertexCount = strLength * 6;

        return 1;
    }
    else {
        printf("  alloc_text(): failed to alloc %zd bytes for text !\n", strLength + 1);
        return 0;
    }
}

int text_set_vbos(Text* text)
{
    // if (!text || !text->_dirty)
    //     return;

    // if (text->_vertices)
    // {
    //     text->vertexCount = 0;

    //     free(text->_vertices);
    //     text->_vertices = GLT_NULL;
    // }

    // if (!text->_text || !text->_textLength)
    // {
    //     text->_dirty = GL_FALSE;
    //     return;
    // }

    // const GLsizei countDrawable = gltCountDrawableCharacters(text->_text);

    // if (!countDrawable)
    // {
    //     text->_dirty = GL_FALSE;
    //     return;
    // }
    // const GLsizei vertexCount = countDrawable * 2 * 3; // 3 vertices in a triangle, 2 triangles in a quad

    // if (!text || !text->vertexCount) {
    if (!text) {
        printf("  alloc_text_buffers(): input param NULL or has 0 vertex count!\n");
        return 0;
    }


    // const GLsizei vertexSize = TEXT2D_VERTEX_ATTR_SIZE;
    // GLfloat *vertices = (GLfloat*)malloc(vertexCount * vertexSize * sizeof(GLfloat));

    // GLfloat* vertices = (GLfloat*)malloc(text->vertexCount * TEXT2D_VERTEX_ATTR_SIZE * sizeof(GLfloat));
    // if (!vertices) {
    //     printf("  alloc_text_buffers(): faile to alloc %d * %d * %zd bytes for vertices info !\n",
    //         text->vertexCount, TEXT2D_VERTEX_ATTR_SIZE, sizeof(GLfloat));
    //     return 0;
    // }

    {
        // img_2d_dimension textDimension = { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f };
        img_2d_dimension textDimension = { TEXT_TEX_CHAR_WIDTH, TEXT_TEX_CHAR_HEIHGT, 0.0f, 0.0f, 0.0f };
        // img_2d_dimension textDimension = { 640 / 2, 480 / 2, 0.0f, 0.0f, 0.0f };

        text->vertexCount = 6;

        float vert_pos[6 * 3];  // 6 points per char, 3 floats for each point
        calculate_6_rect_vert_pos(vert_pos, textDimension.w, textDimension.h);
        if (!vertBuffData(text->_vbo[0], sizeof(float) * (6 * 3), vert_pos)) {
            return 0;
        }

        float vert_uv[6 * 2];   // 6 points per char, 2 floats for each point
        calculate_6_rect_vert_uv(vert_uv, 1, 2, uvWidth, uvHeight);
        if (!vertBuffData(text->_vbo[1], sizeof(float) * (6 * 2), vert_uv)) {
            return 0;
        }
    }

    return 1;



    //     GLsizei vertexElementIndex = 0;

    //     GLfloat glyphX = 0.0f;
    //     GLfloat glyphY = 0.0f;

    //     GLfloat glyphWidth;
    //     const GLfloat glyphHeight = (GLfloat)_gltFontGlyphHeight;

    //     const GLfloat glyphAdvanceX = 0.0f;
    //     const GLfloat glyphAdvanceY = 0.0f;

    //     _GLTglyph glyph;

    //     char c;
    //     int i;
    //     for (i = 0; i < text->_length; i++)
    //     {
    //         c = text->_text[i];

    //         if (c == '\n')
    //         {
    //             glyphX = 0.0f;
    //             glyphY += glyphHeight + glyphAdvanceY;

    //             continue;
    //         }
    //         else if (c == '\r')
    //         {
    //             glyphX = 0.0f;

    //             continue;
    //         }

    //         if (!gltIsCharacterSupported(c))
    //         {
    // #ifdef GLT_UNKNOWN_CHARACTER
    //             c = GLT_UNKNOWN_CHARACTER;
    //             if (!gltIsCharacterSupported(c))
    //                 continue;
    // #else
    //             continue;
    // #endif
    //         }

    //         glyph = _gltFontGlyphs2[c - _gltFontGlyphMinChar];

    //         glyphWidth = (GLfloat)glyph.w;

    //         if (glyph.drawable)
    //         {
    //             vertices[vertexElementIndex++] = glyphX;
    //             vertices[vertexElementIndex++] = glyphY;
    //             vertices[vertexElementIndex++] = glyph.u1;
    //             vertices[vertexElementIndex++] = glyph.v1;

    //             vertices[vertexElementIndex++] = glyphX + glyphWidth;
    //             vertices[vertexElementIndex++] = glyphY + glyphHeight;
    //             vertices[vertexElementIndex++] = glyph.u2;
    //             vertices[vertexElementIndex++] = glyph.v2;

    //             vertices[vertexElementIndex++] = glyphX + glyphWidth;
    //             vertices[vertexElementIndex++] = glyphY;
    //             vertices[vertexElementIndex++] = glyph.u2;
    //             vertices[vertexElementIndex++] = glyph.v1;

    //             vertices[vertexElementIndex++] = glyphX;
    //             vertices[vertexElementIndex++] = glyphY;
    //             vertices[vertexElementIndex++] = glyph.u1;
    //             vertices[vertexElementIndex++] = glyph.v1;

    //             vertices[vertexElementIndex++] = glyphX;
    //             vertices[vertexElementIndex++] = glyphY + glyphHeight;
    //             vertices[vertexElementIndex++] = glyph.u1;
    //             vertices[vertexElementIndex++] = glyph.v2;

    //             vertices[vertexElementIndex++] = glyphX + glyphWidth;
    //             vertices[vertexElementIndex++] = glyphY + glyphHeight;
    //             vertices[vertexElementIndex++] = glyph.u2;
    //             vertices[vertexElementIndex++] = glyph.v2;
    //         }

    //         glyphX += glyphWidth + glyphAdvanceX;
    //     }

    //     text->vertexCount = vertexCount;
    //     text->_vertices = vertices;

    //     glBindBuffer(GL_ARRAY_BUFFER, text->_vbo);
    //     glBufferData(GL_ARRAY_BUFFER, vertexCount * _GLT_TEXT2D_VERTEX_SIZE * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

    //     text->_dirty = GL_FALSE;
}

int text_set_char(Text* text, const char *c, unsigned int font_width, unsigned int font_height)
{
    if (!text) {
        printf("  alloc_text_buffers(): input param NULL or has 0 vertex count!\n");
        return 0;
    }

    {
        text->vertexCount = 6;

        //img_2d_dimension textDimension = { font_width, font_height, 0.0f, 0.0f, 0.0f };
        // img_2d_dimension textDimension = { 
        //     TEXT_TEX_CHAR_WIDTH, TEXT_TEX_CHAR_HEIHGT, 
        //     transX, transY, rotZ 
        // };

        // Calculate vert pos attr according to 'textDimension'
        float vert_pos[6 * 3];  // 6 points per char, 3 floats for each point
        // calculate_6_rect_vert_pos(vert_pos, textDimension.w, textDimension.h);
        calculate_6_rect_vert_pos(vert_pos, font_width, font_height);

        if (!vertBuffData(text->_vbo[0], sizeof(float) * (6 * 3), vert_pos)) {
            return 0;
        }

        // Calculate uv attr according to input char 
        float vert_uv[6 * 2];   // 6 points per char, 2 floats for each point
        if ( *c >= 'a' && *c <= 'z' ) {
            unsigned int offset = *c - 'a';
            unsigned int offset_a = 17;
            unsigned int cols_per_row = 13;

            unsigned int row = (offset_a + offset) / cols_per_row;
            unsigned int col = (offset_a + offset) % cols_per_row;

            calculate_6_rect_vert_uv(vert_uv, row, col, uvWidth, uvHeight);
        }
        else if (*c >= 'A' && *c <= 'Z') {
            unsigned int offset = *c - 'A';
            unsigned int offset_A = 17;
            unsigned int cols_per_row = 13;

            unsigned int row = (offset_A + offset) / cols_per_row;
            unsigned int col = (offset_A + offset) % cols_per_row;

            calculate_6_rect_vert_uv(vert_uv, row, col, uvWidth, uvHeight);
        }
        else if (*c >= '0' && *c <= '9') {
            unsigned int offset = *c - '0';
            unsigned int offset_0 = 0;
            unsigned int cols_per_row = 13;

            unsigned int row = (offset_0 + offset) / cols_per_row;
            unsigned int col = (offset_0 + offset) % cols_per_row;
            
            calculate_6_rect_vert_uv(vert_uv, row, col, uvWidth, uvHeight);
        }
        else {
            switch (*c) {
                case '<':
                calculate_6_rect_vert_uv(vert_uv, 1, 12, uvWidth, uvHeight);  // ? for other symbols
                break;

                case '=':
                calculate_6_rect_vert_uv(vert_uv, 2, 0, uvWidth, uvHeight);  // ? for other symbols
                break;

                case '>':
                calculate_6_rect_vert_uv(vert_uv, 2, 1, uvWidth, uvHeight);  // ? for other symbols
                break;

                case ' ':
                calculate_6_rect_vert_uv(vert_uv, 3, 10, uvWidth, uvHeight);  // ? for other symbols
                break;

                default:
                calculate_6_rect_vert_uv(vert_uv, 1, 2, uvWidth, uvHeight);  // ? for other symbols
                break;
            }
        }

        if (!vertBuffData(text->_vbo[1], sizeof(float) * (6 * 2), vert_uv)) {
            return 0;
        }
    }

    return 1;
}

// #define _gltDrawText() \
//     glUniformMatrix4fv(_text2DShaderMVPUniformLocation, 1, GL_FALSE, mvp); \
//     \
//     glBindVertexArray(text->_vao); \
//     glDrawArrays(GL_TRIANGLES, 0, text->vertexCount);

void begin_draw_text(unsigned int progId, unsigned int texId) {
    glUseProgram(progId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
}

// void color_text(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
//     glUniform4f(_text2DShaderColorUniformLocation, r, g, b, a);
// }

void draw_text(unsigned int prog, Text *text, const glm::mat4 *projection2D_Mat, GLfloat x, GLfloat y, GLfloat scale) {
    if (!text)
        return;

    // if (text->_dirty)
    //     _gltUpdateBuffers(text);

    if (!text->vertexCount)
        return;

    // #ifndef GLT_MANUAL_VIEWPORT
    //     GLint viewportWidth, viewportHeight;
    //     _gltGetViewportSize(&viewportWidth, &viewportHeight);
    //     gltViewport(viewportWidth, viewportHeight);
    // #endif

    //     const GLfloat model[16] = {
    //         scale, 0.0f, 0.0f, 0.0f,
    //         0.0f, scale, 0.0f, 0.0f,
    //         0.0f, 0.0f, scale, 0.0f,
    //            x,    y,  0.0f, 1.0f,
    //     };

    //     GLfloat mvp[16];
    //     _gltMat4Mult(_text2DProjectionMatrix, model, mvp);

    //     _gltDrawText();
        // glUniformMatrix4fv(_text2DShaderMVPUniformLocation, 1, GL_FALSE, mvp);

        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
        glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));

        glm::mat4 model_matrix = transMat * rotMat * scaleMat;
        glm::mat4 mvp = *projection2D_Mat * model_matrix;

        shader_update_mvp(prog, &mvp[0][0]);
        // shader_update_alpha(imgShader, alpha);

    // glBindVertexArray(text->_vao);
    vertBuffAttrPtr(text->_vbo[0], TEXT2D_POSITION_ATTR_IDX, TEXT2D_POSITION_ATTR_SIZE, TEXT2D_POSITION_ATTR_SIZE * sizeof(float), 0);  // pos attr
    vertBuffAttrPtr(text->_vbo[1], TEXT2D_TEXCOORD_ATTR_IDX, TEXT2D_TEXCOORD_ATTR_SIZE, TEXT2D_TEXCOORD_ATTR_SIZE * sizeof(float), 0);  // uv  attr
        glDrawArrays(GL_TRIANGLES, 0, text->vertexCount);
}

void text_draw_vbos(Text *text) {
    if (!text)
        return;

    if (!text->vertexCount)
        return;

    // glBindVertexArray(text->_vao);
    vertBuffAttrPtr(text->_vbo[0], TEXT2D_POSITION_ATTR_IDX, TEXT2D_POSITION_ATTR_SIZE, TEXT2D_POSITION_ATTR_SIZE * sizeof(float), 0);  // pos attr
    vertBuffAttrPtr(text->_vbo[1], TEXT2D_TEXCOORD_ATTR_IDX, TEXT2D_TEXCOORD_ATTR_SIZE, TEXT2D_TEXCOORD_ATTR_SIZE * sizeof(float), 0);  // uv  attr
        glDrawArrays(GL_TRIANGLES, 0, text->vertexCount);
}
