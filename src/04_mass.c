#include "raylib.h"
#include "raymath.h"

#define VEC(x, y) ((Vector2) {x, y})

static const int screen_width = 320;
static const int screen_height= 256;

typedef struct {
    float radius;
    float mass;
    Vector2 position;
    Vector2 velocity; // current velocity
    Vector2 acceleration; // velocity to achive
} Mover;

static void apply_force(Vector2* acceleration, Vector2 force) {
    *acceleration = Vector2Add(*acceleration, force);
}

static void erase_force(Vector2* acceleration) {
    *acceleration = VEC(0.0, 0.0);
}

#define MOVERS_LEN 5
static Mover movers[MOVERS_LEN];
static Vector2 wind_force;
static Vector2 gravity;

static void init() {
    for (int i = 0; i < MOVERS_LEN; i++) {
        movers[i].mass = GetRandomValue(1, 5);
        movers[i].radius = movers[i].mass * 5;
        movers[i].position = VEC( GetRandomValue(movers[i].radius, screen_width*0.5-movers[i].radius), screen_height*0.5f );
        movers[i].velocity = VEC(0.0, 0.0);
        movers[i].acceleration = VEC(0.0, 0.0);
    }

    wind_force = VEC(80.0, 0.0);
    gravity = VEC(0.0, 100.0);
}

static void update() {
    for (int i = 0; i < MOVERS_LEN; ++i) {
        Vector2 scaled_wind_force = Vector2Scale(wind_force, 1.0/movers[i].mass);
        apply_force(&movers[i].acceleration, Vector2Scale(scaled_wind_force, GetFrameTime()));

        // gravity is constant
        apply_force(&movers[i].acceleration, Vector2Scale(gravity, GetFrameTime()));

        movers[i].velocity = Vector2Add(movers[i].velocity, Vector2Scale(movers[i].acceleration, GetFrameTime()));
        movers[i].position = Vector2Add(movers[i].position, movers[i].velocity);

        // collision to wall  
        if (movers[i].position.x >= screen_width-movers[i].radius || movers[i].position.x <= 0+movers[i].radius) {
            movers[i].velocity.x *= -1; // reverse direction
        }

        if (movers[i].position.y >= screen_height-movers[i].radius || movers[i].position.y <= 0+movers[i].radius) {
            movers[i].velocity.y *= -1; // reverse direction
        }

        // wipe out acceleration
        erase_force(&movers[i].acceleration);
    }
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < MOVERS_LEN; ++i) {
        DrawCircleV(movers[i].position, movers[i].radius, ColorAlpha(RED, 0.4));
    }
    
    EndDrawing();
}

int main() {
    InitWindow(screen_width, screen_height, "Force");
    SetTargetFPS(60);

    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();

}
