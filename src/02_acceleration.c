#include "raylib.h"
#include "raymath.h"

#define VEC(x, y) ((Vector2) {x, y})

#define SPEED           250.0f
#define PLAYER_RADIUS   12.0f

static const int screen_width = 320;
static const int screen_height= 256;

static Vector2 player_position;
static Vector2 player_velocity; // current velocity
static Vector2 player_acceleration; // velocity to achive

static void init() {
    player_position = (Vector2) { screen_width*0.5f, screen_height*0.5f};
    player_velocity = VEC(0.0, 0.0);
}

static void update() {
    if (IsKeyDown(KEY_W)) player_acceleration      = VEC(0.0, -SPEED);
    else if (IsKeyDown(KEY_S)) player_acceleration = VEC(0.0, SPEED);
    else if (IsKeyDown(KEY_A)) player_acceleration = VEC(-SPEED, 0.0);
    else if (IsKeyDown(KEY_D)) player_acceleration = VEC(SPEED, 0.0);
    else player_acceleration = VEC(0.0, 0.0);

    player_velocity = Vector2Lerp(player_velocity, player_acceleration, 5.0 * GetFrameTime());
    player_position = Vector2Add(player_position, Vector2Scale(player_velocity, GetFrameTime()));

    // stoping enemy from moving out of screen
    player_position = Vector2Clamp( player_position, 
        (Vector2){0+PLAYER_RADIUS,0+PLAYER_RADIUS},
        (Vector2){screen_width-PLAYER_RADIUS, screen_height-PLAYER_RADIUS}
    );
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawCircleV(player_position, PLAYER_RADIUS, BLUE);
    
    EndDrawing();
}

int main() {
    InitWindow(screen_width, screen_height, "Acceleration");
    SetTargetFPS(60);

    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();

}
