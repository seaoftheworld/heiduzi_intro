#include "test.h"

// Include 'GLM' to use glm::translate(), rotate(), normalize() ...
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static GLuint vertBuffer = 0, uvBuffer = 0;

// Quad Co-Ordinates
static GLfloat vert_buffer[] =
{
    // -1.0f,  1.0f, 0.0f,  // left  top      -
    // -1.0f, -1.0f, 0.0f,  // left  bottom   |\ 
    //  1.0f, -1.0f, 0.0f,  // right bottom   |_\

    //  1.0f, -1.0f, 0.0f,  // right bottom   __
    //  1.0f,  1.0f, 0.0f,  // right top      \ |
    // -1.0f,  1.0f, 0.0f,  // left  top       \|

    // -640.0f,  480.0f, 0.0f,  // left  top      -
    // -640.0f, -480.0f, 0.0f,  // left  bottom   |\ 
    //  640.0f, -480.0f, 0.0f,  // right bottom   |_\

    //  640.0f, -480.0f, 0.0f,  // right bottom   __
    //  640.0f,  480.0f, 0.0f,  // right top      \ |
    // -640.0f,  480.0f, 0.0f,  // left  top       \|

    -320.0f,  240.0f, 0.0f,  // left  top      -
    -320.0f, -240.0f, 0.0f,  // left  bottom   |\ 
     320.0f, -240.0f, 0.0f,  // right bottom   |_\

     320.0f, -240.0f, 0.0f,  // right bottom   __
     320.0f,  240.0f, 0.0f,  // right top      \ |
    -320.0f,  240.0f, 0.0f,  // left  top       \|
};

static GLfloat uv_buffer[] =
{
    0.0f, 0.0f,  // left  top        -   
    0.0f, 1.0f,  // left  bottom     |\ 
    1.0f, 1.0f,  // right bottom     |_\

    1.0f, 1.0f,  // right  bottom  __
    1.0f, 0.0f,  // right  top     \ |
    0.0f, 0.0f,  // left   top      \|
};


void deinitVertBuffs(unsigned int num, unsigned int *vbo) {
    if (num) {
        glDeleteBuffers(num, vbo);
    }
}

void initVertBuffs(unsigned int num, unsigned int *vbo) {
    if (num) {
        glGenBuffers(num, vbo);
    }
}

void deinitVertBuff(unsigned int *vbo) {
    glDeleteBuffers(1, vbo);
}

void initVertBuff(unsigned int *vbo) {
    glGenBuffers(1, vbo);
}

int vertBuffData(unsigned int vbo, unsigned int size, const void *data) {
    if (!vbo) {
        printf("vertBuffData(): vbo does not exist\n");
        return 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return 1;
}





int vertBuffData_old(GLuint vbo, GLsizeiptr size, const void *data) {
    if (!vbo) {
        printf("vertBuffData(): vbo does not exist\n");
        return 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    return 1;
}

void vertBuffAttrPtr(GLuint vbo, GLuint attrIdx, GLint attrSize, GLsizei attrStride, GLintptr vboOffset) {
    if (!vbo) {
        printf("vertBuffAttr(): vbo does not exist\n");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(attrIdx);
    
        glVertexAttribPointer(attrIdx, 
            attrSize, GL_FLOAT, 
            GL_FALSE, 
            attrStride, 
            (void *)vboOffset);
    
    // glEnableVertexAttribArray(attrib.idx);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void bindVertBuff(GLuint vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void unbindVertBuff() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}




void vertBuffTest_Deinit() {
    deinitVertBuff(&vertBuffer);
    deinitVertBuff(&uvBuffer);

    vertBuffer = 0;
    uvBuffer = 0;

    printf("vertBuffTest_Deinit(): done.\n");
}

int vertBuffTest_Init() {
    initVertBuff(&vertBuffer);
    initVertBuff(&uvBuffer);

    // To make the rotation correct (propotional) without camera, the width and height of the window must be the same,
    // otherwise, when (800, 400) (conner-point's X to center is twice than Y to center), 
    // the rotation cause the value changed on X twice than changed on Y
    // (shall be equal), causing rectangle to become diomand.
    //
    // glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
    // modelMat = glm::rotate(modelMat, (3.14f / 6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // //
    // glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));
    // glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), (3.14f / 12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // glm::mat4 modelMat = transMat * rotMat;

    // glm::mat4 modelMat = glm::translate(glm::mat4(1.0), act->position);
    // modelMat = glm::rotate(modelMat, act->yAngle, glm::vec3(0.f, 0.f, 1.f));

    // for (unsigned char i = 0; i < (3 * 6); i += 3) {
    //     glm::vec3 posAfterRotation = glm::vec3(modelMat * glm::vec4(vert_buffer[i + 0], vert_buffer[i + 1], vert_buffer[i + 2], 1.0f));
    //     vert_buffer[i + 0] = posAfterRotation.x;
    //     vert_buffer[i + 1] = posAfterRotation.y;
    //     vert_buffer[i + 2] = posAfterRotation.z;
    // }

    // if (!vertBuffData(vertBuffer, sizeof(vert_buffer), vert_buffer)) {
    //    return 0;
    // }

    // if (!vertBuffData(uvBuffer, sizeof(uv_buffer), uv_buffer)) {
    //     return 0;
    // }

    printf("vertBuffTest_Init(): done.\n");
    return 1;
}

void vertBuffTest_Draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    vertBuffAttrPtr(vertBuffer, 0, 3, 3 * sizeof(GLfloat), 0);
    vertBuffAttrPtr(uvBuffer, 1, 2, 2 * sizeof(GLfloat), 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// void vertBuffAttr(unsigned int numAttr, vertAttr *attr) {
//     // glClear(GL_COLOR_BUFFER_BIT);
//     for (unsigned int i = 0; i < numAttr; i++) {
//         vertBuffAttrPtr(
//             attr[i].vboId, 
//             attr[i].attrIdx, 
//             attr[i].numFloat, 
//             attr[i].stride,
//             attr[i].offset
//         );
//     }

//     // glDrawArrays(GL_TRIANGLES, 0, 6);
// }
