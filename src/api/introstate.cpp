#include "introstate.h"

// Thess data shall be read in from data files, 
// and shall return error if reading fails.
// static img_2d_dimension read_in_dimensions[TMAX] = {
//     { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f }, 

//     { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 32), -3.14f / 48.0f },
//     { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 32),  3.14f / 48.0f },
//     { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 32), -3.14f / 48.0f },
//     { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 32),  3.14f / 48.0f },

//     // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 64), -3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 64),  3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 16), -3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 16),  3.14f / 48.0f }

//     // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) + (float)(viewportHeight / 16), -3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) + (float)(viewportHeight / 16),  3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 8), -3.14f / 48.0f },
//     // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 8),  3.14f / 48.0f }

//     { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f }
// };

static std::string __imgPaths[TMAX] = {
    "./data/tex/png/road.png",
    
    "./data/tex/png/touch_belly.png",
    "./data/tex/png/feed.png",
    "./data/tex/png/on_lawn.png",
    "./data/tex/png/winter_hide.png",

    "./data/tex/png/background.png"
};
static std::string __charMap = "./data/tex/png/font.png";
// static const char __info[] = "Press Left to start  press right to quit";
// static const char __info[] = "Press here to start   press here to quit";
// static const char __info[] = "Hello Heiduzi         press here to quit";
// static const char __info[] = "Thanks for testing    press here to quit";

   // static const char __info[] = "                      press here to quit";  // draw 'quit' with varable-alpha first
   // static const char __info[] = "Thanks for testing";                        // then draw this with fixed alpha
   static const char __info[] = " Thanks for testing  press here to quit";

static void calculate_6_rect_vert_pos(float *rect_vert_3_6_float, unsigned int expectedWidth, unsigned int expectedHeight) {
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

static void draw_1st_screen(introstate *obj, float alpha) {
    obj->draw_picture(T00, alpha, 0.0f);
}

static void draw_2nd_screen(introstate *obj, texVboEnum pic, float alpha) {
    int i = pic;
    
    if (i == T00 || i == TMAX) {
    }
    else if (i == T05) {
        obj->draw_picture(T05, alpha, 0.0f);
    }
    else {
        texVboEnum pics[TMAX] = { TMAX, T01, T02, T03, T04, TMAX };

        obj->draw_picture(T05, 0.4f, 0.0f);

        // printf("00 i: %d\n", i);
        obj->draw_picture(pics[i--], alpha, 0.0f);  // Draw the indexed pic first with passed-in alpha, then reduce the index by one,
        for (; pics[i] != TMAX; i--) {                    // and draw the indexed pic with alpha == 1, ...
            // printf("01 i: %d\n", i);
            obj->draw_picture(pics[i], 1.0f, 0.0f);
        }
    }
}

static void draw_3rd_screen(introstate *obj, float up) {
    obj->draw_picture(T05, 0.4f, 0.0f);

    obj->draw_picture(T04, 1.0f, up);
    obj->draw_picture(T03, 1.0f, up);
    obj->draw_picture(T02, 1.0f, up);
    obj->draw_picture(T01, 1.0f, up);
}

void introstate::draw_picture(texVboEnum pic, float alpha, float up) {
    // glClear(GL_COLOR_BUFFER_BIT);

    bindTex(GL_TEXTURE0, imgTexIds[pic]);

    vertBuffAttrPtr(imgPosBuffIds[pic], 0, 3, 3 * sizeof(float), 0);
    vertBuffAttrPtr( imgUvBuffId, 1, 2, 2 * sizeof(float), 0);

        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(
            imgDimensions[pic].transX, 
            imgDimensions[pic].transY + viewportHeight * up, 
            0.0f));
        glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), imgDimensions[pic].rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        
        glm::mat4 model_matrix = transMat * rotMat * scaleMat;
        glm::mat4 mvp = projection2D_Mat * model_matrix;

        shader_update_mvp(imgShader, &mvp[0][0]);
        shader_update_alpha(imgShader, alpha);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void introstate::draw_text(unsigned int prog, unsigned int tex, 
    const glm::mat4 *projection2D_Mat, 
    unsigned int offset, unsigned int count,
    float xStart, float yStart, 
    float scaleX, float scaleY, float alpha) {

    bind_shader(prog);
    bindTex(GL_TEXTURE0, tex);

    float x = xStart;
    float y = yStart;

    // for (unsigned int i = 0; i < MAX_NUM_TEXT; i++) {
    //     if (!tsText[i]._vbo[0] || !tsText[i]._vbo[1]) {
    //         break;
    //     }

    //     // draw_text(textShader, &tsText[i], &projection2D_Mat, x, y, 1.0f);
    //         glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    //         // glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    //         glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));

    //         // glm::mat4 model_matrix = transMat * rotMat * scaleMat;
    //         glm::mat4 model_matrix = transMat * scaleMat;
    //         glm::mat4 mvp = *projection2D_Mat * model_matrix;

    //         shader_update_mvp(prog, &mvp[0][0]);
    //         shader_update_alpha(prog, alpha);

    //     text_draw_vbos(&tsText[i]);
    //     x += TEXT_TEX_CHAR_WIDTH * scaleX;
    // }

    for (unsigned int i = offset; i < MAX_NUM_TEXT && (i - offset) < count; i++) {
        if (!tsText[i]._vbo[0] || !tsText[i]._vbo[1]) {
            break;
        }

        // draw_text(textShader, &tsText[i], &projection2D_Mat, x, y, 1.0f);
            glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
            // glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));

            // glm::mat4 model_matrix = transMat * rotMat * scaleMat;
            glm::mat4 model_matrix = transMat * scaleMat;
            glm::mat4 mvp = *projection2D_Mat * model_matrix;

            shader_update_mvp(prog, &mvp[0][0]);
            shader_update_alpha(prog, alpha);

        text_draw_vbos(&tsText[i]);
        x += TEXT_TEX_CHAR_WIDTH * scaleX;
    }
}

#if TEST_DRAW_FULL_CHAR_MAP
void introstate::draw_full_char_map(float up) {
    // glClear(GL_COLOR_BUFFER_BIT);
    bindTex(GL_TEXTURE0, charMapTexId);

    vertBuffAttrPtr(charMapPosBuff, 0, 3, 3 * sizeof(float), 0);
    // vertBuffAttrPtr(  imgUvBuffId, 1, 2, 2 * sizeof(float), 0);
    vertBuffAttrPtr(  imgUvQuatBuff, 1, 2, 2 * sizeof(float), 0);

        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(
            charMapDimension.transX, 
            charMapDimension.transY + viewportHeight * up, 
            0.0f));
        glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), charMapDimension.rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        
        glm::mat4 model_matrix = transMat * rotMat * scaleMat;
        glm::mat4 mvp = projection2D_Mat * model_matrix;

        shader_update_mvp(textShader, &mvp[0][0]);
        // shader_update_alpha(imgShader, alpha);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
#endif

int introstate::Create() {
    // Get the correct value for 'viewportWidth' and 'viewportHeight' of the displaying window, 
    // and calculate the 'projection2D_Mat' accordingly.
    {
        GLint result[4]; 
        glGetIntegerv(GL_VIEWPORT, result);
        viewportWidth = result[2]; viewportHeight = result[3];
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            return 0;
        }

        // 'projection2D_Mat' Referenced from 'glText' codes: gltViewport()
        const float left   = -(float)viewportWidth / 2.0f;
        const float bottom = -(float)viewportHeight / 2.0f;
        const float right  =  (float)viewportWidth / 2.0f;
        const float top    =  (float)viewportHeight / 2.0f;
        const float zNear = -1.0f;
        const float zFar  = 1.0f;

        projection2D_Mat = {
            (2.0f / (right - left)), 0.0f, 0.0f, 0.0f,
            0.0f, (2.0f / (top - bottom)), 0.0f, 0.0f,
            0.0f, 0.0f, (-2.0f / (zFar - zNear)), 0.0f,

            -((right + left) / (right - left)),
            -((top + bottom) / (top - bottom)),
            -((zFar + zNear) / (zFar - zNear)),
            1.0f
        };
    }

    // Set the width, height, center-pos and rotation for each picture to be displayed.
    // Thess data shall be read in from data files, 
    // and shall return error if reading fails.
    {
        img_2d_dimension read_in_dimensions[TMAX] = {
            { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f }, 

            { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 32), -3.14f / 48.0f },
            { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 32),  3.14f / 48.0f },
            { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 32), -3.14f / 48.0f },
            { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 32),  3.14f / 48.0f },

            // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 64), -3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 64),  3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 16), -3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 16),  3.14f / 48.0f }

            // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) + (float)(viewportHeight / 16), -3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32),  (float)(viewportHeight / 4) + (float)(viewportHeight / 16),  3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2,  (float)(viewportWidth / 4) - (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 8), -3.14f / 48.0f },
            // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32), -(float)(viewportHeight / 4) + (float)(viewportHeight / 8),  3.14f / 48.0f }

            // { viewportWidth / 2, viewportHeight / 2, -(float)(viewportWidth / 4) + (float)(viewportWidth / 32),  (float)(viewportHeight / 4) - (float)(viewportHeight / 32), -3.14f / 48.0f }
            { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f }
        };

        for (unsigned int i = T00; i < TMAX; i++) {
            imgDimensions[i] = read_in_dimensions[i];
        }
    }

    // Init image-shader and text-shader
    {
        std::string shaderName = "image_mod";
        imgShader = load_shader(shaderName);
        if ( !imgShader ) {
            return 0;
        }
        
        shaderName = "text";
        textShader = load_shader(shaderName);
        if ( !textShader ) {
            return 0;
        }
    }

    // Init image-textures and char-map texture for text.
    {
        std::string read_in_imgPath[TMAX];
        for (unsigned int i = T00; i < TMAX; i++) {
            read_in_imgPath[i] = __imgPaths[i];
        }

        if ( !createTex(TMAX, imgTexIds, read_in_imgPath) ) {
            return 0;
        }

        std::string read_in_charMap = __charMap;
        if ( !createTex(1, &charMapTexId, &read_in_charMap) ) {
            return 0;
        }
    }

    // Init the vbos for shader's attributes
    //... ...

    // Init vbos for text display
    {
        // unsigned int strLen = strlen(__info);
        size_t strLen = strlen(__info);
        if (!strLen) {
            return 0;
        }

        for (unsigned int i = 0; i < strLen && i < MAX_NUM_TEXT; i++) {
            if (!text_gen_vbos(&tsText[i])) {
                return 0;
            }

            if (!text_set_char(&tsText[i], &__info[i], TEXT_TEX_CHAR_WIDTH, TEXT_TEX_CHAR_HEIHGT)) {
                return 0;
            }
        }
        
        // if (!init_text(&tsText)) {
        //     return 0;
        // }
        // // if (!alloc_text(&tsText, "safafdasfd")) {
        // //     return 0;
        // // }
        // if (!alloc_text_buffers(&tsText)) {
        //     return 0;
        // }
    }

    // Init vbos for images display
    // Init the 'imgPosBuffIds' (from 'imgDimensions') according to expected data 
    // and init 'imgUvBuffId' to  display full image.
    {
        // for (unsigned int i = T00; i < TMAX; i++) {
        //     imgDimensions[i] = read_in_dimensions[i];
        // }

        // Generate vbos for img-shader's attr: vert_pos/uv, for each picture to be displayed
        initVertBuffs(TMAX, imgPosBuffIds);
        for (unsigned int i = T00; i < TMAX; i++) {

            float vert_pos[6 * 3];
            calculate_6_rect_vert_pos(vert_pos, imgDimensions[i].w, imgDimensions[i].h);
            
            // initVertBuffs(TMAX, imgPosBuffIds);  // This is wrong !!!
            // initVertBuff(imgPosBuffIds + i);
            if ( !vertBuffData(imgPosBuffIds[i], sizeof(float) * (6 * 3), vert_pos) ) {
                return 0;
            }
        }

        float uv_buffer[] = {
            0.0f, 0.0f,  // left  top        -   
            0.0f, 1.0f,  // left  bottom     |\ 
            1.0f, 1.0f,  // right bottom     |_\

            1.0f, 1.0f,  // right  bottom  __
            1.0f, 0.0f,  // right  top     \ |
            0.0f, 0.0f,  // left   top      \|
        };
        initVertBuff(&imgUvBuffId);
        if ( !vertBuffData(imgUvBuffId, sizeof(float) * (2 * 6), uv_buffer) ) {
            return 0;
        }
    }

    // Init vbos for charMap display test.
    // Init 'charMapPosBuff' from 'charMapDimension' to test draw_full_char_map(), and 
    // init 'imgUvQuatBuff' for additional test of draw_full_char_map()
    #if TEST_DRAW_FULL_CHAR_MAP
    {
        charMapDimension = { viewportWidth, viewportHeight, 0.0f, 0.0f, 0.0f };
        // charMapDimension = { viewportWidth / 2, viewportHeight / 2, 0.0f, 0.0f, 0.0f };

        float vert_pos[6 * 3];
        calculate_6_rect_vert_pos(vert_pos, charMapDimension.w, charMapDimension.h);

        initVertBuffs(1, &charMapPosBuff);
        if ( !vertBuffData(charMapPosBuff, sizeof(float) * (3 * 6), vert_pos) ) {
            return 0;
        }

        float uv_buffer_quat[] = {
            0.0f, 0.0f,  // left  top        -   
            0.0f, 0.5f,  // left  bottom     |\ 
            0.5f, 0.5f,  // right bottom     |_\

            0.5f, 0.5f,  // right  bottom  __
            0.5f, 0.0f,  // right  top     \ |
            0.0f, 0.0f,  // left   top      \|
        };
            // float uv_buffer_quat[] = {
            //     0.0f, 0.0f,  // left  top        -   
            //     0.0f, 0.148f,  // left  bottom     |\ 
            //     0.074f, 0.148f,  // right bottom     |_\

            //     0.074f, 0.148f,  // right  bottom  __
            //     0.074f, 0.0f,  // right  top     \ |
            //     0.0f, 0.0f,  // left   top      \|
            // };
        initVertBuff(&imgUvQuatBuff);
        if ( !vertBuffData(imgUvQuatBuff, sizeof(float) * (2 * 6), uv_buffer_quat) ) {
            return 0;
        }
    }
    #endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

    running = TRUE;
    return 1;
}

void introstate::Destroy() {
    // deinit_text(&tsText);
    for (unsigned int i = 0; i < MAX_NUM_TEXT; i++) {
        text_del_vbos(&tsText[i]);
    }

    deinitVertBuff(&imgUvBuffId);
    deinitVertBuffs(TMAX, imgPosBuffIds);

    #if TEST_DRAW_FULL_CHAR_MAP
        deinitVertBuff(&imgUvQuatBuff);
        deinitVertBuff(&charMapPosBuff);
    #endif

    destroyTex(1, &charMapTexId);
    destroyTex(TMAX, imgTexIds);

    unload_shader(imgShader);
    unload_shader(textShader);

    running = FALSE;
}

void introstate::InputHandle(const void *data_handle) {
    // Tobe simplified like sh menu style
    static bool first = FALSE;

    if (!data_handle) {
        // return;
        first = TRUE;
    }
    else {
        double (*data)[2] = (double (*)[2])data_handle;
        printf("  mouse clicked: %4f, %4f\n", (*data)[0], (*data)[1]);

        if (first) {
            first = FALSE;

            // if ( (*data)[0] >= ((float)viewportWidth * 11.0 / 20.0) && (*data)[0] < ((float)viewportWidth * 78.0 / 80.0)) {
            //     printf("  mouse clicked: xxxxx\n");
            // }
            // if ( (*data)[1] >= ((float)viewportHeight * 67.0 / 75.0) && (*data)[1] < ((float)viewportHeight * 72.0 / 75.0) ) {
            //     printf("  mouse clicked: yyyyy\n");
            // }
                // if ( (*data)[0] >= ((float)viewportWidth * 11.0 / 20.0) && (*data)[0] < ((float)viewportWidth * 39.0 / 40.0) &&
                //      (*data)[1] >= ((float)viewportHeight * 67.0 / 75.0) && (*data)[1] < ((float)viewportHeight * 72.0 / 75.0) &&
                //      screen == scrShowStart ) {
                //     running = FALSE;
                // }

            // if ( (*data)[0] >= ((float)viewportWidth * 42.0 / 80.0) && (*data)[0] < ((float)viewportWidth * 75.0 / 80.0)) {
            //     printf("  mouse clicked: xxxxx\n");
            // }
            // if ( (*data)[1] >= ((float)viewportHeight * 67.0 / 75.0) && (*data)[1] < ((float)viewportHeight * 72.0 / 75.0) ) {
            //     printf("  mouse clicked: yyyyy\n");
            // }
            if ( (*data)[0] >= ((float)viewportWidth * 21.0 / 40.0) && (*data)[0] < ((float)viewportWidth * 15.0 / 16.0) &&
                 (*data)[1] >= ((float)viewportHeight * 67.0 / 75.0) && (*data)[1] < ((float)viewportHeight * 72.0 / 75.0) &&
                 screen == scrShowStart ) {
                running = FALSE;
            }
        }
    }
}

void introstate::Update(float delta) {
    /*
    static float elapasedTime = 0;
    elapasedTime = elapasedTime + delta;
    // printf("\n\n  __ update(): elapasedTime-%f\n\n", elapasedTime);

    if (elapasedTime < 1.0f && !startProcess) {
        // shader_update_alpha(0.0f);
        // shader_update_alpha(1.0f);
        // alpha = 1.0f;
        targetTexVboIdx = T00;
        alpha = 0.0f;

        startProcess = 1;
    }
    else if (elapasedTime >= 1.0f && elapasedTime <= 2.0) {
        // float alpha = elapasedTime - 1.0f;
        // shader_update_alpha(alpha);
        alpha = elapasedTime - 1.0f;
    }
    else if (elapasedTime >= 3.0f && elapasedTime <= 4.0f) {
        // float alpha = 4.0f - elapasedTime;
        // shader_update_alpha(alpha);
        alpha = 4.0f - elapasedTime;
    }
    else if (elapasedTime > 4.0f && !endProcess) {
        // shader_update_alpha(0.0f);
        
        alpha = 0.0f;
        endProcess = 1;
    }

    // glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)(viewportWidth / 32), 0.0f, 0.0f));
    // glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), (3.14f / 12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // glm::mat4 modelMat = transMat * rotMat;

    // glm::mat4 mvp = projection2D_Mat * model_matrix[targetTexVboIdx];
    // shader_update_mvp(&mvp[0][0]);

    if (elapasedTime < 4.5) {
        targetTexVboIdx = T00;
    }
    else if (elapasedTime >= 4.5f && elapasedTime < 5.0f) {
        alpha = 1.0f * (elapasedTime - 4.5f) / 0.5f;
        targetTexVboIdx = T01;   
    }
    else if (elapasedTime >= 5.0f && elapasedTime < 5.5f) {
        alpha = 1.0f * (elapasedTime - 5.0f) / 0.5f;
        targetTexVboIdx = T02;
    }
    else if (elapasedTime >= 5.5f && elapasedTime < 6.0f) {
        alpha = 1.0f * (elapasedTime - 5.5f) / 0.5f;
        targetTexVboIdx = T03;
    }
    else if (elapasedTime >= 6.0f && elapasedTime < 6.5f) {
        alpha = 1.0f * (elapasedTime - 6.0f) / 0.5f;
        targetTexVboIdx = T04;
    }
    else if (elapasedTime >= 6.5f) {
        targetTexVboIdx = TMAX;
    }

    if (elapasedTime >= 6.5f && elapasedTime < 7.0f) {
        alpha = 0.15f * (elapasedTime - 6.5f) / 0.5f;
    }
    else {
    }
    //*/
    
    // update() is now called at fixed interval (25Hz, 40ms once),
    // so it's no longer up to time to update logic, 
    // it's up to ticks insteak. 
    // 25 ticks means 1-second has passed.
    //*
    #define TICKS_TO_FLOAT   ((float)ticks)  // to compare with 1.5/2.5/... s
    #define TICKS_ADD        (ticks++)
    #define TICKS_PER_SECOND (50)
    #define TICKS_FOR_SECONDS(n) ((float)n * TICKS_PER_SECOND)

    static bool first = 1; // , endProcess = 0;
    if (first) {
        first = 0;
        // targetTexVboIdx = T00;
        alpha = 0.0f;
    }

    // Could also change according to 'screen' value,
    // ticks can be reset everytime screen value changes
    //... ...

    // Changing according to ticks
    if ( TICKS_TO_FLOAT < TICKS_FOR_SECONDS(4.5f) ) {

        if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(1.0f) && 
            TICKS_TO_FLOAT < TICKS_FOR_SECONDS(2.0f)) {
            alpha = 1.0f - (TICKS_FOR_SECONDS(2.0f) - TICKS_TO_FLOAT) / TICKS_PER_SECOND;
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(3.0f) && 
                 TICKS_TO_FLOAT <= TICKS_FOR_SECONDS(4.0f)) {
            alpha = 1.0f - (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(3.0f)) / TICKS_PER_SECOND;
        }
        // else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(4.0f) && !endProcess) {
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(4.0f)) {
            alpha = 0.0f;
            // endProcess = 1;
        }

        screen = scrShowIntro;
    }
    else if ( TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(4.5f) && TICKS_TO_FLOAT < TICKS_FOR_SECONDS(7.0f) ) {

        if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(4.5f) && 
            TICKS_TO_FLOAT < TICKS_FOR_SECONDS(5.0f)) {
            alpha = 0.4f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(4.5f)) / TICKS_FOR_SECONDS(0.5f);
            targetTexVboIdx = T05;   
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(5.0f) && 
            TICKS_TO_FLOAT < TICKS_FOR_SECONDS(5.5f)) {
            // alpha = 1.0f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(4.5f)) / TICKS_FOR_SECONDS(0.5f);
            alpha = 1.0f - (TICKS_FOR_SECONDS(5.5f) - TICKS_TO_FLOAT) / TICKS_FOR_SECONDS(0.5f);
            targetTexVboIdx = T01;   
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(5.5f) && 
                 TICKS_TO_FLOAT < TICKS_FOR_SECONDS(6.0f)) {
            alpha = 1.0f - (TICKS_FOR_SECONDS(6.0f) - TICKS_TO_FLOAT) / TICKS_FOR_SECONDS(0.5f);
            targetTexVboIdx = T02;
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(6.0f) && 
                 TICKS_TO_FLOAT < TICKS_FOR_SECONDS(6.5f)) {
            alpha = 1.0f - (TICKS_FOR_SECONDS(6.5f) - TICKS_TO_FLOAT) / TICKS_FOR_SECONDS(0.5f);
            targetTexVboIdx = T03;
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(6.5f) && 
                 TICKS_TO_FLOAT < TICKS_FOR_SECONDS(7.0f)) {
            alpha = 1.0f - (TICKS_FOR_SECONDS(7.0f) - TICKS_TO_FLOAT) / TICKS_FOR_SECONDS(0.5f);
            targetTexVboIdx = T04;
        }

        screen = scrShowDogs;
    }
    else if ( TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(7.0f) && TICKS_TO_FLOAT < TICKS_FOR_SECONDS(7.5f) ) {
        // 'alpha' is used as 'up'
        // alpha = 0.15f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(7.0f)) / TICKS_FOR_SECONDS(0.5f);
        up = 0.15f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(7.0f)) / TICKS_FOR_SECONDS(0.5f);
        screen = scrShowStart;
    }
    else {
        //  7.5s alpha 1.0f,  8.5s alpha 0.0,
        //  9.5s alpha 1.0f, 10.5s alpha 0.0,

        if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(7.5f) && 
            TICKS_TO_FLOAT < TICKS_FOR_SECONDS(8.0f)) {
            // alpha = 0.4f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(4.5f)) / TICKS_FOR_SECONDS(0.5f);      // 0.5s
            alpha = 1.0f - (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(7.5f)) / TICKS_FOR_SECONDS(0.5f * 1.2);   // alpha decrease from 1 to (1 - 1/1.2 == 0.17).
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(8.0f) && 
                 TICKS_TO_FLOAT < TICKS_FOR_SECONDS(8.2f)) {
            // alpha = 1.0f * (TICKS_TO_FLOAT - TICKS_FOR_SECONDS(4.5f)) / TICKS_FOR_SECONDS(0.5f);     // 0.2s
            alpha = 1.0f - (TICKS_FOR_SECONDS(8.2f) - TICKS_TO_FLOAT) / TICKS_FOR_SECONDS(0.2f * 1.2);  // alpha increase from (1 - 1/1.2 == 0.17) to 1.
        }
        else if (TICKS_TO_FLOAT >= TICKS_FOR_SECONDS(8.2f) && 
                 TICKS_TO_FLOAT < TICKS_FOR_SECONDS(9.0f)) {
            // keep alpha == 1.0 for another 1.0s
        }
        else {
            // TICKS_TO_FLOAT = TICKS_FOR_SECONDS(7.5);
            // ticks = TICKS_FOR_SECONDS(7.5);
            ticks = 7.5 * TICKS_PER_SECOND;
        }
    }

    TICKS_ADD;
    //*/
}

void introstate::Render() {
    // Set the shaders to use
    bind_shader(imgShader);

    // glClear(GL_COLOR_BUFFER_BIT);

    // // Bind the texture to a slot, and Set the Attr for the shader from vbos
    // // texEnum targetTexVboIdx = T00;
    // bindTex(GL_TEXTURE0, imgTexIds[targetTexVboIdx]);

    // vertBuffAttrPtr(imgPosBuffIds[targetTexVboIdx], 0, 3, 3 * sizeof(float), 0);
    // vertBuffAttrPtr(                    imgUvBuffId, 1, 2, 2 * sizeof(float), 0);

    // // glm::mat4 mvp = projection2D_Mat * model_matrix[targetTexVboIdx];
    // // shader_update_mvp(&mvp[0][0]);

    // // Draw
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    // glClear(GL_COLOR_BUFFER_BIT);
    // draw_pictures(this, T02, alpha);
    
    glClear(GL_COLOR_BUFFER_BIT);
    // draw_picture(T01, alpha);

    // draw_picture(T02, alpha);
    //     draw_picture(T01, alpha);

    // draw_picture(T03, alpha);
    //     draw_picture(T02, alpha);
    //     draw_picture(T01, alpha);

    // draw_picture(T01, alpha);
    // draw_picture(T02, alpha);
    // draw_picture(T03, alpha);
    // draw_picture(T04, alpha);

    // draw_picture(T04, alpha, 0.25f);
    // draw_picture(T03, alpha, 0.25f);
    // draw_picture(T02, alpha, 0.25f);
    // draw_picture(T01, alpha, 0.25f);

    // draw_3rd_screen(this, 0.25f);
    // draw_2nd_screen(this, T01, 1.0f);
    /*
    if (targetTexVboIdx == T00) {
        draw_1st_screen(this, alpha);
    }
    else if (targetTexVboIdx == T05 || targetTexVboIdx == TMAX) {
        draw_3rd_screen(this, alpha);  // alpha used as up-value
    }
    else {
        draw_2nd_screen(this, targetTexVboIdx, alpha);
    }
    //*/

    //*
    switch (screen) {
        case scrShowIntro:
        draw_1st_screen(this, alpha);
        break;

        case scrShowDogs:
        draw_2nd_screen(this, targetTexVboIdx, alpha);
        break;

        case scrShowStart:
        // draw_3rd_screen(this, alpha);  // alpha is used as up-value here
        draw_3rd_screen(this, up);

        #if TEST_DRAW_FULL_CHAR_MAP
            bind_shader(textShader);
            draw_full_char_map(0.0f);
        #endif

            {
                // bind_shader(textShader);
                // bindTex(GL_TEXTURE0, charMapTexId);

                // // draw_text(textShader, &tsText, &projection2D_Mat, -((float)viewportWidth / 4), -((float)viewportHeight * 7 / 16), 1.0f);
                // float x = -((float)viewportWidth * 15 / 32);
                // float y = -((float)viewportHeight * 7 / 16);

                // for (unsigned int i = 0; i < MAX_NUM_TEXT; i++) {
                //     if (!tsText[i]._vbo[0] || !tsText[i]._vbo[1]) {
                //         break;
                //     }

                //     draw_text(textShader, &tsText[i], &projection2D_Mat, x, y, 1.0f);
                //     x += TEXT_TEX_CHAR_WIDTH;
                // }

                // draw_text(textShader, charMapTexId, &projection2D_Mat, 
                //     -((float)viewportWidth * 15 / 32), 
                //     -((float)viewportHeight * 7 / 16), 
                //       (float)viewportWidth/800.0f, (float)viewportWidth/600.0f, alpha);



                // Draw 'quit' part with varable alpha first. Then draw 
                // another part of the text with fixed-alpha.
                float scaleX = (float)viewportWidth / 800.0f,
                      scaleY = (float)viewportHeight / 600.0f;
                
                unsigned int char_offset = 21,
                             char_count = 18;

                draw_text(textShader, charMapTexId, &projection2D_Mat, 
                    char_offset, char_count,
                    -((float)viewportWidth * 15 / 32) + char_offset * TEXT_TEX_CHAR_WIDTH * scaleX, 
                    -((float)viewportHeight * 7 / 16), 
                    scaleX, scaleY, alpha);

                char_offset = 0; 
                char_count = 19;
                draw_text(textShader, charMapTexId, &projection2D_Mat, 
                    char_offset, char_count,
                    -((float)viewportWidth * 15 / 32), 
                    -((float)viewportHeight * 7 / 16), 
                    scaleX, scaleY, 1.0f);
            }


        break;

        default:
        break;
    }
    //*/

    // draw_picture(T05, 0.5f, 1.0f);
    // draw_3rd_screen(this, 0.0f); 
}

// static GLfloat vertBuff_00[] =
// {
//     -320.0f,  240.0f, 0.0f,  // left  top      -
//     -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
//      320.0f, -240.0f, 0.0f,  // right bottom   |_\

//      320.0f, -240.0f, 0.0f,  // right bottom   __
//      320.0f,  240.0f, 0.0f,  // right top      \ |
//     -320.0f,  240.0f, 0.0f,  // left  top       \|
// };

// static GLfloat vertBuff_01[] =
// {
//     -320.0f,  240.0f, 0.0f,  // left  top      -
//     -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
//      320.0f, -240.0f, 0.0f,  // right bottom   |_\

//      320.0f, -240.0f, 0.0f,  // right bottom   __
//      320.0f,  240.0f, 0.0f,  // right top      \ |
//     -320.0f,  240.0f, 0.0f,  // left  top       \|
// };

// static GLfloat vertBuff_02[] =
// {
//     -320.0f,  240.0f, 0.0f,  // left  top      -
//     -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
//      320.0f, -240.0f, 0.0f,  // right bottom   |_\

//      320.0f, -240.0f, 0.0f,  // right bottom   __
//      320.0f,  240.0f, 0.0f,  // right top      \ |
//     -320.0f,  240.0f, 0.0f,  // left  top       \|
// };

// static GLfloat vertBuff_03[] =
// {
//     -320.0f,  240.0f, 0.0f,  // left  top      -
//     -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
//      320.0f, -240.0f, 0.0f,  // right bottom   |_\

//      320.0f, -240.0f, 0.0f,  // right bottom   __
//      320.0f,  240.0f, 0.0f,  // right top      \ |
//     -320.0f,  240.0f, 0.0f,  // left  top       \|
// };

// static GLfloat vertBuff_04[] =
// {
//     -320.0f,  240.0f, 0.0f,  // left  top      -
//     -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
//      320.0f, -240.0f, 0.0f,  // right bottom   |_\

//      320.0f, -240.0f, 0.0f,  // right bottom   __
//      320.0f,  240.0f, 0.0f,  // right top      \ |
//     -320.0f,  240.0f, 0.0f,  // left  top       \|
// };

// static float *vertBuffs[TMAX] = {
//     vertBuff_00, vertBuff_01, vertBuff_02,
//     vertBuff_03, vertBuff_04
// };