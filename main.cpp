#include <windows.h>
#include <mmsystem.h>

#include "gamewindow.h"


// Main cycle
int main() {

    // Play music (only .wav files)
    //PlaySound(TEXT(R"(sounds\test.wav)"), nullptr, SND_FILENAME | SND_ASYNC);

    // Creates window
    const int WINDOW_WIDTH = 1440;
    const int WINDOW_HEIGHT = 900;

    Camera cam(glm::vec3(0.f, 0.35f, 1.f), glm::vec3(0.f, 1.f, 0.f));
    GameWindow window(WINDOW_WIDTH, WINDOW_HEIGHT, "a", cam);

    // Loops until window gets closed
    while (!glfwWindowShouldClose(window.get_window())) {

        window.update();
        window.render();

        glUseProgram(0);
    }

    glfwDestroyWindow(window.get_window());
    glfwTerminate();
    //glDeleteProgram(program);
    return 0;
}