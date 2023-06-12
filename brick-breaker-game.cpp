#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

int main(void);

void processInput(GLFWwindow* window);

enum BRICKTYPE { DESTRUCTIBLE, PADDLE };
enum ONOFF { ON, OFF };


class Brick
{
public:
    float red, green, blue;
    float x, y, width;
    BRICKTYPE brick_type;
    ONOFF onoff;
    int health; // Tracks the number of times the brick has been hit

    Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
    {
        brick_type = bt;
        x = xx;
        y = yy;
        width = ww;
        red = rr;
        green = gg;
        blue = bb;
        onoff = ON;
        health = 3; // Initialize health to 3
    }

    void drawBrick()
    {
        if (onoff == ON)
        {
            double halfside = width / 2;

            glColor3d(red, green, blue);
            glBegin(GL_POLYGON);

            glVertex2d(x + halfside, y + halfside);
            glVertex2d(x + halfside, y - halfside);
            glVertex2d(x - halfside, y - halfside);
            glVertex2d(x - halfside, y + halfside);

            glEnd();
        }
    }
};
Brick paddle(PADDLE, -0.0, -1.0, 0.2, 1, 0, 0);
class Circle
{
public:
    float red, green, blue;
    float radius;
    float x;
    float y;
    float speed = 0.02; // alter the speed
    int direction; // 1=up 2=right 3=down 4=left 5 = up right   6 = up left  7 = down right  8= down left


    Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
    {
        x = xx;
        y = yy;
        radius = rr;
        red = r;
        green = g;
        blue = b;
        radius = rad;
        direction = dir;
    }

    void CheckCollision(Brick* brk)
    {
        if (brk->onoff == OFF) {
            return; // Skip collision detection if the brick is already turned off
        }

        float brickLeft = brk->x - brk->width;
        float brickRight = brk->x + brk->width;
        float brickBottom = brk->y - brk->width;
        float brickTop = brk->y + brk->width;

        if (x + radius >= brickLeft && x - radius <= brickRight && y + radius >= brickBottom && y - radius <= brickTop)
        {
            if (brk->brick_type == PADDLE)
            {
                direction = GetRandomDirection();
                x += 0.03f;
                y += 0.04f;
            }
            else if (brk->brick_type == DESTRUCTIBLE && brk->health >= 0)
            {
                brk->health--; // Update the health of the brick
                if (brk->health <= 0) // Check if health is less than or equal to 0
                {
                    brk->onoff = OFF;
                    return;
                }
                //changes to red if the circle hits the brick
                brk->green = 0.0;
                brk->blue = 0.0;

            }

        }
    }


    int GetRandomDirection()
    {
        return (rand() % 8) + 1;
    }

    void MoveOneStep()
    {
        if (direction == 1 || direction == 5 || direction == 6)  // up
        {
            if (y > -1 + radius)
            {
                y -= speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 2 || direction == 5 || direction == 7)  // right
        {
            if (x < 1 - radius)
            {
                x += speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 3 || direction == 7 || direction == 8)  // down
        {
            if (y < 1 - radius) {
                y += speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }

        if (direction == 4 || direction == 6 || direction == 8)  // left
        {
            if (x > -1 + radius) {
                x -= speed;
            }
            else
            {
                direction = GetRandomDirection();
            }
        }
    }

    void DrawCircle()
    {
        glColor3f(red, green, blue);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float degInRad = i * DEG2RAD;
            glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
        }
        glEnd();
    }
};

vector<Circle> world;

int main(void) {
    srand(time(NULL));

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(480, 480, "Brick Breaker", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Destructible bricks on the top
    Brick brick1(DESTRUCTIBLE, -1.0, 0.5, 0.2, 1, 1, 1);
    Brick brick2(DESTRUCTIBLE, -0.8, 0.5, 0.2, 1, 1, 1);
    Brick brick3(DESTRUCTIBLE, -0.6, 0.5, 0.2, 1, 1, 1);
    Brick brick4(DESTRUCTIBLE, -0.4, 0.5, 0.2, 1, 1, 1);
    Brick brick5(DESTRUCTIBLE, -0.2, 0.5, 0.2, 1, 1, 1);
    Brick brick6(DESTRUCTIBLE, -0.0, 0.5, 0.2, 1, 1, 1);
    Brick brick7(DESTRUCTIBLE, 0.2, 0.5, 0.2, 1, 1, 1);
    Brick brick8(DESTRUCTIBLE, 0.4, 0.5, 0.2, 1, 1, 1);
    Brick brick9(DESTRUCTIBLE, 0.6, 0.5, 0.2, 1, 1, 1);
    Brick brick10(DESTRUCTIBLE, 0.8, 0.5, 0.2, 1, 1, 1);
    Brick brick11(DESTRUCTIBLE, 1.0, 0.5, 0.2, 1, 1, 1);



    while (!glfwWindowShouldClose(window)) {
        // Setup View
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);


        for (size_t i = 0; i < world.size(); i++)
        {
            // Check collision with bricks
            world[i].CheckCollision(&paddle);
            world[i].CheckCollision(&brick1);
            world[i].CheckCollision(&brick2);
            world[i].CheckCollision(&brick3);
            world[i].CheckCollision(&brick4);
            world[i].CheckCollision(&brick5);
            world[i].CheckCollision(&brick6);
            world[i].CheckCollision(&brick7);
            world[i].CheckCollision(&brick8);
            world[i].CheckCollision(&brick9);
            world[i].CheckCollision(&brick10);
            world[i].CheckCollision(&brick11);

            // Move the circle
            world[i].MoveOneStep();

            // Draw the circle
            world[i].DrawCircle();

            // Check collision between circles
            for (int j = 0; j < world.size(); j++)
            {
                Circle* ptrA = &world[i];
                Circle* ptrB = &world[j];

                if (ptrA != ptrB)
                {
                    // Check if circles overlap
                    if (((world[i].x > world[j].x - world[j].radius && world[i].x <= world[j].x + world[j].radius)
                        && (world[i].y > world[j].y - world[j].radius && world[i].y <= world[j].y + world[j].radius)))
                    {
                        // Remove circle i if overlap occurs
                        world.erase(world.begin() + i);
                        break;
                    }
                }
            }
        }

        // Draw bricks
        paddle.drawBrick();
        brick1.drawBrick();
        brick2.drawBrick();
        brick3.drawBrick();
        brick4.drawBrick();
        brick5.drawBrick();
        brick6.drawBrick();
        brick7.drawBrick();
        brick8.drawBrick();
        brick9.drawBrick();
        brick10.drawBrick();
        brick11.drawBrick();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        double r, g, b;
        r = rand() / 10000;
        g = rand() / 10000;
        b = rand() / 10000;
        // the circles come out of a paddle
        Circle B(paddle.x, paddle.y + 0.02, 0.05, 1, 0.05, r, g, b);
        B.direction = B.GetRandomDirection();
        world.push_back(B);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && paddle.x > -1.0f)
        paddle.x -= 0.05;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && paddle.x < 1.0f)
        paddle.x += 0.05;
}