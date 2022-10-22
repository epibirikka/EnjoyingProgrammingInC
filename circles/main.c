#include <stdio.h>
#include "raylib.h"
#include "ball.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

const Vector2 win_size = {800, 600};

int main()
{
	srand(time(NULL));

	InitWindow(win_size.x, win_size.y, "balls orbiting");

	Camera camera = {0};
	camera.position = (Vector3){0.0f, 10.0f, 10.0f};
	camera.target = (Vector3){0.0f, 0.0f, 0.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 50.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	float timer, dt;
	timer = 0;

	SetCameraMode(camera, CAMERA_ORBITAL);

	Ball balls[20];

	for (int i=0; i<20; ++i)
	{
		balls[i] = ball_new();
	}

	while (!WindowShouldClose())
	{
		dt = GetFrameTime();
		timer += dt;

		UpdateCamera(&camera);
		
		for (int i=0; i<20; ++i)
		{
			ball_update(&balls[i], dt);
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);
	
		// 3d!!
		BeginMode3D(camera);

		for (int i=0; i<20; ++i)
		{
			ball_draw(&balls[i]);
		}

		DrawGrid(50, 10.0f);

		EndMode3D();

		// text
		DrawFPS(10, win_size.y - 30);

		char p[30];
		sprintf(p, "%.2fS", timer);
		DrawText(p, win_size.x - MeasureText(p, 20) - 10, win_size.y - 30, 20, DARKGREEN);
		

		EndDrawing();
	}	

	CloseWindow();
}
