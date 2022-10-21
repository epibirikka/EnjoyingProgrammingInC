// LIL TEST

#include <stdio.h>
#include <raylib.h>
#include <string.h>

int main()
{
    Vector2 window_size = {800, 600};
    Rectangle rect = {0, 0, 100, 100};
    Vector2 vel = {1, 1};

    const float speed = 150;

    InitWindow(window_size.x, window_size.y, "hello!");

    float dt, timer;
    int bumps = 0;

    timer = 0;

    while (!WindowShouldClose())
    {
        dt = GetFrameTime();
        timer += dt;

        // collisions

        rect.x += vel.x * speed * dt;
        rect.y += vel.y * speed * dt;

        if (rect.x + rect.width > window_size.x || rect.x < 0)
        {
            if (rect.x > window_size.x * 0.5)
            {
                rect.x = window_size.x - rect.width;
            }
            else
            {
                rect.x = 0;
            }

            vel.x *= -1;
            bumps++;
        }
        
        if (rect.y + rect.height > window_size.y || rect.y < 0)
        {
            if (rect.y > window_size.y * 0.5)
            {
                rect.y = window_size.y - rect.height;
            }
            else
            {
                rect.y = 0;
            }

            vel.y *= -1;
            bumps++;
        }

        //

        // draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("RAYLIB BOUNCING RECTANGLE DEMO", 10, 10, 18, LIGHTGRAY);

            char p[20];
            sprintf(p, "PROJECT TIMER: %.3fs", timer);
            DrawText(p, 10, 40, 18, LIGHTGRAY);

            sprintf(p, "BUMPS: %d", bumps);
            DrawText(p, 10, 60, 18, LIGHTGRAY);

            DrawRectangle(rect.x, rect.y, rect.width, rect.height, MAROON);
        EndDrawing();
    }

    CloseWindow();

    return 0;    
}