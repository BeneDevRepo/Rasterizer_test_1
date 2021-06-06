#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "Window.h"
#include "Keyboard.h"

#include "Mesh.h"

int main() {
    Window win(800, 600);

    float vertices[] {
         0.5f,-0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f
    };

    unsigned int indices[] {
        0, 2, 3,
        0, 1, 2,
        0, 5, 4,
        0, 1, 5,
        4, 0, 3,
        4, 3, 7,
        4, 5, 6,
        6, 7, 4,
        5, 1, 2,
        2, 6, 5,
        3, 2, 6,
        3, 6, 7
    };

    Mesh mesh(vertices, 8, indices, 12);

    POINT lastMouse = {0, 0};
    while (!win.shouldClose) {
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

        win.graphics.clear(0xff0000ff);// A R G B
        // win.graphics.line(0, 0, mousePos.x, mousePos.y, 0xffff00ff);
        // win.graphics.line(win.width/2, win.height/2, mousePos.x, mousePos.y, 0xffff00ff);
        mesh.drawTo(&(win.graphics));
        win.updateScreen();

        lastMouse = mousePos;
    }
    return 0;
}