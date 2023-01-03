#include "raylib.h"
#include "raymath.h"

#define VEC(x, y) ((Vector2) {x, y})

static const int screen_width = 320; // 256 * 1.25
static const int screen_height= 256;

static Vector2 player_position;
static Vector2 player_velocity; // current velocity
static const float SPEED = 250.0f; // pixel per second
static const float RADIUS = 10.0f;

static void init() {
    player_position = (Vector2) { screen_width*0.5f, screen_height*0.5f};
    player_velocity = VEC(SPEED, SPEED);
}

static void update() {
    player_position = Vector2Add(player_position, Vector2Scale(player_velocity, GetFrameTime()));

    if (player_position.x >= screen_width-RADIUS || player_position.x <= 0+RADIUS) {
        player_velocity.x *= -1; // reverse direction
    }

    if (player_position.y >= screen_height-RADIUS || player_position.y <= 0+RADIUS) {
        player_velocity.y *= -1; // reverse direction
    }
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawCircleV(player_position, RADIUS, BLUE);
    
    EndDrawing();
}

int main() {
    InitWindow(screen_width, screen_height, "Bouncing Ball");
    SetTargetFPS(60);

    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();
}
