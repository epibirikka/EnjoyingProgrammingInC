#include "math.h"
#include "raylib.h"
#include "ball.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

Ball ball_new()
{
	Ball n_ball;
	n_ball.target_position = (Vector3){0.0f, 0.0f, 0.0f};
	n_ball.target_radius = 2.5 + fmod(rand(), 7);
	n_ball.orbit = fmod(rand(), PI * 2);
	n_ball.z_rotate = fmod(rand(), PI * 2);
	n_ball.position = (Vector3) {0.0f, 0.0f, 0.0f};
	n_ball.color = (Color) {
			128 + (rand() * 96),
			128 + (rand() * 96),
			128 + (rand() * 96)
		, 255};

	n_ball.sphere_radius = 0.5f + fmod(rand(), 0.75f);
	n_ball.orbit_speed = 1 + fmod(rand(), 3);

	if (fmod(rand(), 2) >= 1)
	{
		n_ball.orbit_speed *= -1;
	}

	return n_ball;
}

void ball_update(Ball *ball, float dt)
{
	ball->orbit += ball->orbit_speed * dt;
	ball->z_rotate += ball->orbit_speed * dt * 0.75;

	ball->position = (Vector3) {cos(ball->orbit) * ball->target_radius, cos(ball->z_rotate) * ball->target_radius, sin(ball->orbit) * ball->target_radius};

}

void ball_draw(Ball *ball)
{
	DrawSphere(ball->position, ball->sphere_radius, ball->color);
}
