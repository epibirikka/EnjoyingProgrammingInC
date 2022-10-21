#include <stdio.h>
#include "raylib.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>


typedef struct Paddle
{
    float start_x;
    float move_y;
    float y_limit;
    float y;
    float start_y;
    int score;
    Rectangle collision;
} Paddle;

typedef struct Ball
{
    Rectangle rect;
    Vector2 start_position;
    Vector2 bounce_bounds;
    Vector2 velocity;
    Vector2 center;
    Paddle* assign_paddles[2];
    float inactive_timer;
    float active_timer;
    int bumps;
    int ended;
} Ball;

const Vector2 w_size = {800, 600};

// PADDLE
void paddle_reset(Paddle* p)
{
    p->y = p->start_y;
}

Paddle paddle_init(float start_x, float y, float y_limit)
{
    Paddle n_p;
    n_p.start_x = start_x;
    n_p.y_limit = y_limit;
    n_p.score = 0;
    n_p.collision.width = 15;
    n_p.collision.height = 100;
    n_p.start_y = y - (n_p.collision.height * 0.5);
    paddle_reset(&n_p);

    return n_p;
}

void paddle_input(Paddle *p)
{
    p->move_y = IsKeyDown(KEY_UP) ? -1.0f : (IsKeyDown(KEY_DOWN) ? 1.0f : 0.0f);
}

void paddle_ai_control(Paddle *p, Ball *ball, float react_limit)
{
    if (abs(ball->rect.x - p->start_x) > react_limit)
    {
        p->move_y = 0;
        return;
    }
    
    float p_cheight = p->collision.height * 0.5;
    float p_center_x =  p->collision.y + p_cheight;
    
    p->move_y = (p_center_x > ball->center.y) ? -1 : 1;
}

const float PADDLE_SPEED = 500;

void paddle_update(Paddle *p, float dt)
{
    p->y += p->move_y * dt * PADDLE_SPEED;
    p->collision.x = p->start_x;
    p->collision.y = p->y;

    float y = p->y;
    float height = p->collision.height;

    if (y < 0)
    {
        p->y = 0;
    }
    else if (y + height > p->y_limit)
    {
        p->y = p->y_limit - height;
    }
}

void paddle_draw(Paddle *p)
{
    Rectangle r = p->collision;

    DrawRectangle(r.x, r.y, r.width, r.height, WHITE);
}

//

// BALL
const float BALL_INIT_RADIUS = 10;
const float BALL_INACTIVE_TIMER_ASSIGN = 1;
const float BALL_ACTIVE_TIMER_ASSIGN = 30;
const float BALL_SPEED = 400;
const int BALL_BUMPS_ASSIGN = 50;

void ball_reset(Ball *ball)
{
    ball->inactive_timer = BALL_INACTIVE_TIMER_ASSIGN;
    ball->active_timer = BALL_ACTIVE_TIMER_ASSIGN;
    ball->rect.x = ball->start_position.x;
    ball->rect.y = ball->start_position.y;
    ball->bumps = BALL_BUMPS_ASSIGN;
}

Ball ball_init(Vector2 start_position, Vector2 bounce_bounds)
{
    Ball n_ball;
    n_ball.start_position = (Vector2){start_position.x - (BALL_INIT_RADIUS * 0.5), start_position.y - (BALL_INIT_RADIUS * 0.5)};
    n_ball.bounce_bounds = bounce_bounds;
    n_ball.rect.width = BALL_INIT_RADIUS;
    n_ball.rect.height = BALL_INIT_RADIUS;
    n_ball.ended = 0;

    ball_reset(&n_ball);

    return n_ball;
}

void ball_rotate(Ball *ball, float angle)
{
    ball->velocity.x = cos(angle);
    ball->velocity.y = sin(angle);
}

const int GOAL = 5;

int ball_game_finished(Ball *ball)
{
    for (int i=0; i<2; ++i)
    {
        if (ball->assign_paddles[i]->score < GOAL)
        {
            continue;
        }

        return 1;
    }

    return 0;
}

void ball_update(Ball *ball, float dt)
{
    if (ball->inactive_timer > 0)
    {
        ball->inactive_timer -= dt;

        if (ball->inactive_timer <= 0)
        {
            if (ball_game_finished(ball) == 1)
            {
                ball->ended = 1;
            }
            else
            {
                ball_rotate(ball, fmod(rand(), PI * 2));
            }
        }

        return;
    }
    
    if (ball->ended == 1)
        return;

    ball->active_timer -= dt;

    ball->rect.x += ball->velocity.x * BALL_SPEED * dt;
    ball->rect.y += ball->velocity.y * BALL_SPEED * dt;

    float x = ball->rect.x;
    float y = ball->rect.y;
    float width = ball->rect.width;
    float height = ball->rect.height;

    int bounce_x = 0;
    int bounce_y = 0;

    if (ball->active_timer <= 0 || 
            x + width < 0 || x + width > ball->bounce_bounds.x)
    {
        // score
        if (ball->active_timer > 0 && ball->bumps > 0)
        {
            int assign_index = (x > ball->bounce_bounds.x * 0.5) ? 0 : 1;
            Paddle *p = ball->assign_paddles[assign_index];
            p->score++;
        }

        for (int i=0; i<2; i++)
        {
            paddle_reset(ball->assign_paddles[i]);
        }

        ball_reset(ball);
    }
 
    if (y < 0)
    {
        ball->rect.y = 0;
        bounce_y = 1;
    }

    if (y + height > ball->bounce_bounds.y)
    {
        ball->rect.y = ball->bounce_bounds.y - height;
        bounce_y = 1;
    }

    if (bounce_x == 1)
    {
        ball->velocity.x *= -1;   
    }

    if (bounce_y == 1)
    {
        ball->velocity.y *= -1;
    }

    ball->center = (Vector2){ball->rect.x + (BALL_INIT_RADIUS * 0.5),
                                ball->rect.y + (BALL_INIT_RADIUS * 0.5)};
}

void ball_collides_with(Ball *ball, Paddle *p, int angle_flip)
{
    if (CheckCollisionRecs(ball->rect, p->collision) == 0)
    {
        return;
    }

    // bounce

    float p_cheight = p->collision.height * 0.5;
    float p_center_x =  p->collision.y + p_cheight;
    float angle = (p_center_x - ball->center.y) / p_cheight;

    ball_rotate(ball, angle);

    if (angle_flip)
        ball->velocity.x *= -1;

    ball->velocity.y *= -1;

    float slid = ball->velocity.x >= 0 ? 1 : -1;

    while (CheckCollisionRecs(ball->rect, p->collision) == 1)
    {
        ball->rect.x += slid;
    }

    ball->bumps--;
}

void ball_draw(Ball *ball)
{
    DrawCircle(ball->rect.x + (BALL_INIT_RADIUS * 0.5),
                ball->rect.y + (BALL_INIT_RADIUS * 0.5), BALL_INIT_RADIUS, WHITE);
}

//

int main()
{
    srand(time(NULL));

    const int GAP = 150;

    InitWindow(w_size.x, w_size.y, "Pong :D");

    float dt;
    float y_init = w_size.y * 0.5;

    Paddle p1 = paddle_init(GAP, y_init, w_size.y);
    Paddle p2 = paddle_init(w_size.x - GAP, y_init, w_size.y);
    Ball ball = ball_init((Vector2){w_size.x * 0.5, w_size.y * 0.5}, w_size);
    ball.assign_paddles[0] = &p1;
    ball.assign_paddles[1] = &p2;

    float result_transition = 0.0;

    while (!WindowShouldClose())
    {
        dt = GetFrameTime();

        if (ball.inactive_timer <= 0)
        {
            paddle_input(&p1);
            paddle_ai_control(&p2, &ball, w_size.y * 0.25);
        }
        else
        {
            p1.move_y = 0;
            p2.move_y = 0;
        }
        
        paddle_update(&p1, dt);
        paddle_update(&p2, dt);

        ball_update(&ball, dt);
        ball_collides_with(&ball, &p1, 0);
        ball_collides_with(&ball, &p2, 1);

        if (ball.ended == 1)
        {
            result_transition += dt * 0.5;

            if (result_transition > 1)
            {
                result_transition = 1;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            paddle_draw(&p1);
            paddle_draw(&p2);
            ball_draw(&ball);

            char text[10];

            sprintf(text, "%d", p1.score);
            DrawText(text, 50, 15, 40, WHITE);
            
            sprintf(text, "%d", p2.score);
            DrawText(text, w_size.x - 50, 15, 40, WHITE);
            
            sprintf(text, "%.1f", ball.active_timer);
            DrawText(text, (w_size.x * 0.5) - (MeasureText(text, 40) * 0.5), 15, 40, WHITE);
            
            sprintf(text, "%d", ball.bumps);
            DrawText(text, (w_size.x * 0.5) - (MeasureText(text, 40) * 0.5), 55, 40, GRAY);

            if (ball.ended == 1)
            {
                float alpha =  result_transition * 255;

                char *result_text = (p1.score >= GOAL) ? "You win" : "You lost";

                DrawRectangle(0, 0, w_size.x, w_size.y, (Color){255, 255, 255, alpha * 0.45});

                DrawText(result_text, (w_size.x * 0.5) - (MeasureText(result_text, 40) * 0.5), (w_size.y * 0.5) - 20, 40, 
                            (Color){0, 0, 0, alpha});
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}