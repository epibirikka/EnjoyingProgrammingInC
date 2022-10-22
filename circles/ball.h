#include "raylib.h"

typedef struct {
	Vector3 target_position;
	Vector3 position;
	float target_radius;
	float sphere_radius;
	float orbit;
	float orbit_speed;
	float z_rotate;
	Color color;
} Ball;

Ball ball_new();
void ball_update(Ball *ball, float dt);
void ball_draw(Ball *ball);
