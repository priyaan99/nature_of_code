#include "raylib.h"
#include "raymath.h"

#define RADIUS      10.0f
#define LINE_LEN    25.0f

#define VEC(x, y) ((Vector2) {x, y})

static const int screen_width = 320; // 256 * 1.25
static const int screen_height= 256;

static Vector2 player_position;
static Vector2 direction_to_mouse;

static void init() {
    player_position = (Vector2) { screen_width*0.5f, screen_height*0.5f};
}

static void update() {
    direction_to_mouse = Vector2Subtract(GetMousePosition(), player_position);
    direction_to_mouse = Vector2Normalize(direction_to_mouse);
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawCircleV(player_position, RADIUS, RED);
    DrawLineEx(
        player_position,
        Vector2Add(player_position, Vector2Scale(direction_to_mouse, LINE_LEN)),
        5.0f,
        RED
    );
    
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

