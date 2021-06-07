#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "Window.h"
#include "Keyboard.h"

#include "Mesh.h"

#include <chrono>



int main() {
    Window win(800, 600);

    float vertices[] {
        //x    y     z
        -0.5f,-0.5f,-0.5f,//top left front  0
        -0.5f,-0.5f, 0.5f,//top left back   1
         0.5f,-0.5f,-0.5f,//top right front 2
         0.5f,-0.5f, 0.5f,//top right back  3

        -0.5f, 0.5f,-0.5f,//low left front  4
        -0.5f, 0.5f, 0.5f,//low left back   5
         0.5f, 0.5f,-0.5f,//low right front 6
         0.5f, 0.5f, 0.5f //low right back  7
    };

    unsigned int indices[] {
        //front
        0, 2, 6,
        0, 6, 4,

        //back
        3, 1, 5,
        3, 5, 7,

        //right
        2, 3, 7,
        2, 7, 6,

        //left
        1, 0, 4,
        1, 4, 5,

        //top
        1, 3, 2,
        1, 2, 0,

        //bottom
        4, 6, 7,
        4, 7, 5,
    };

    Mesh mesh = loadSTL("../data/Model1.stl");
    // Mesh mesh = loadOBJ("../data/capsule.obj");
    // Mesh mesh(vertices, 8, indices, 12);

    POINT lastMouse = {0, 0};
    int64_t lastTime = 0;
    while (!win.shouldClose) {
        int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int64_t dt_millis = time - lastTime;
        float dt = dt_millis / 1000.;
        float fps = 1000. / dt_millis;
        // std::cout << "" << fps << "\n";
        lastTime = time;

        win.pollMsg();
        Keyboard::poll();
        POINT mousePos = win.getMouse();

        if (Keyboard::pressed('W'))
            std::cout << "W\n";
        if (Keyboard::pressed('A'))
            std::cout << "A\n";
        if (Keyboard::pressed('S'))
            std::cout << "S\n";
        if (Keyboard::pressed('D'))
            std::cout << "D\n";
        if (Keyboard::pressed(0x01)) // left click
            std::cout << "left\n";
        if (Keyboard::pressed(0x02)) // right click
            std::cout << "right\n";

        win.graphics.clear(0xff440088);// A R G B
        mesh.drawTo(&(win.graphics));
        win.updateScreen();

        lastMouse = mousePos;
    }
    return 0;
}