#include <iostream>

// To use OpenGL, Libs depended on are: 
// 1.glfw 2.gl3w 3.glm
#define GLFW_INCLUDE_NONE  // By default, glfw3.h includes <GL/gl.h> for gl1.1 support shipped by vs2019 without this line.
#include <GLFW/glfw3.h>    // Several apis in gl1.1 header will overlap with mordern gl extented by gl3w and cause trouble.

#include <GL/gl3w.h>

#include "api/introstate.h"

// #define WIN_WIDTH (800)
// #define WIN_HEIGHT (600)
#define WIN_WIDTH (640)
#define WIN_HEIGHT (480)

int main() {
	// printf("  __This is a test.\n");

    std::cout << "00. Init 'GLFW' for OpenGL Window and Context" << std::endl;
    if (!glfwInit()) {
        std::cout << "  glfwInit() failed !" << std::endl;

        glfwTerminate();
        return 1;
    }

    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 2);  // 0 has no effect, 1 has effect

    GLFWwindow *m_GLFWWindowPointer = NULL;
    m_GLFWWindowPointer = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "this is a test", NULL, NULL);
    if (!m_GLFWWindowPointer) {
        std::cout << "  glfwCreateWindow() failed !" << std::endl;

        glfwDestroyWindow(m_GLFWWindowPointer);
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(m_GLFWWindowPointer);

    std::cout << "01. OpenGL Window/Context created, Init 'GL3W' for OpenGL extentions" << std::endl;
    if (gl3wInit()) {
        std::cout << "  gl3wInit() failed !" << std::endl;
        glfwDestroyWindow(m_GLFWWindowPointer);
        glfwTerminate();
        return 1;
    }
    std::cout << "  gl3wInit() complete! OpenGL info:" << std::endl;
    std::cout << "    Vendor:  " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "    Render:  " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: " << glGetString(GL_VERSION) << std::endl;

    introstate intro;
    if (!intro.Create()) {
        intro.Destroy();
        
        glfwDestroyWindow(m_GLFWWindowPointer);
        glfwTerminate();
        return 1;
    }

    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(m_GLFWWindowPointer, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);

    double now = 0, last_loop_time = 0, 
           last_update_time = 0, 
           last_render_time = 0, 
           last_1s_time = 0;

    double prevFrameTime = 0;
    // float  elapasedTime = 0;

    // Tobe changed to macros
    // unsigned int update_freq = 30,  // When 'update_freq' < 'render_freq', make sure update() is called
    //              render_freq = 60;  // at least once before render() is called.
    //                                 // Calling render() before update() is called may 
                                       // crash the program due to un-init values.
    #define update_freq (50)
    #define render_freq (50)

    double update_cycle_time = (float)(1.0f / update_freq),
           render_cycle_time = (float)(1.0f / render_freq),
           min_interval = (update_freq > render_freq) ? (float)(1.0f / update_freq) : (float)(1.0f / render_freq);

    unsigned int sleep_times = 0, 
                 wakeup_times = 0, 
                 updating_times = 0, 
                 rendering_times = 0;

    while (!glfwWindowShouldClose(m_GLFWWindowPointer) && intro.isRunning()) {
        static unsigned char first = 1;

        if (first) {
            // printf("000000000000\n");
            now = glfwGetTime();
            last_update_time = now;
            last_render_time = now;
            last_loop_time = now;
            last_1s_time = now;
            first = 0;
        }
        else {  
            // printf("000000000001\n");
            now = glfwGetTime();
            // prevFrameTime = now - last_loop_time;
            // // elapasedTime = elapasedTime + prevFrameTime;  // += crashes
            // last_loop_time = now;
        }

        // Print info to debug and reset debug infos every 1-second
        if (now - last_1s_time > 1.0f) {
            printf("  __ 1s: sleep/wakeup: %d--%d, update/render: %d--%d \n\n", 
                sleep_times, wakeup_times, updating_times, rendering_times);

            if (updating_times < update_freq) {
            }
            if (rendering_times < render_freq) {
            }

            sleep_times = 0;
            wakeup_times = 0;
            updating_times = 0;
            rendering_times = 0;

            last_1s_time = now;
        }

        // When less than the amount of 'min_interval' time has passed since the last valid loop,
        // sleep for 'min_interval' time before the next loop for power-consumption.
        if (now - last_loop_time < min_interval) {
            // sleep() for 'min_interval' time using the OS's api
            //... ...
            // printf("aaa");
            sleep_times++;
            continue;
        }
        else {
            // printf("bbb");
            wakeup_times++;
            last_loop_time = now;
        }

        // When more than the amount of 'min_interval' time has passed since the last valid loop,
        // do the update/render accordingly.

        // printf("1111111111\n");
        if ( now - last_update_time >= update_cycle_time ) {
            // Input handle and Update
            glfwPollEvents();  // without it there's no-respond when close win with mouse

            int mouse_left_press = glfwGetMouseButton(m_GLFWWindowPointer, GLFW_MOUSE_BUTTON_LEFT);
            double xy_pos[2];

            if (mouse_left_press == GLFW_PRESS){
                glfwGetCursorPos(m_GLFWWindowPointer, &xy_pos[0], &xy_pos[1]);
                intro.InputHandle((const void *)xy_pos);
            }
            else {
                intro.InputHandle((const void *)NULL);
            }

            intro.Update((float)(now - last_update_time));

            updating_times++;
            last_update_time = now;
        }

        // printf("2222222222\n");
        if ( now - last_render_time >= render_cycle_time ) {
            // Render
            intro.Render();
            glfwSwapBuffers(m_GLFWWindowPointer);

            rendering_times++;
            last_render_time = now;
        }
    }

    intro.Destroy();

    glfwDestroyWindow(m_GLFWWindowPointer);
    glfwTerminate();
	return 0;
}
