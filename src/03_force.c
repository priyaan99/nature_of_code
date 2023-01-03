#include "raylib.h"
#include "raymath.h"

/// A Force is a vector that causes an object with mass to accelerate
/// * F (force) = M (mass) * A (acceleration)
/// * A = F / M
/// * if M == 1, then A = F

#define VEC(x, y) ((Vector2) {x, y})

#define SPEED           10.0f
#define PLAYER_RADIUS   12.0f

static const int screen_width = 320;
static const int screen_height= 256;

typedef struct {
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

static Mover mover;
static Vector2 force;

static void init() {
    mover.position = VEC( screen_width*0.5f, screen_height*0.5f );
    mover.velocity = VEC(0.0, 0.0);
    mover.acceleration = VEC(0.0, 0.0);

    force = VEC(50.0, 80.0);
}

static void update() {
    apply_force(&mover.acceleration, Vector2Scale(force, GetFrameTime()));

    mover.velocity = Vector2Add(mover.velocity, Vector2Scale(mover.acceleration, GetFrameTime()));
    mover.position = Vector2Add(mover.position, mover.velocity);

    // collision to wall  
    if (mover.position.x >= screen_width-PLAYER_RADIUS || mover.position.x <= 0+PLAYER_RADIUS) {
        mover.velocity.x *= -1; // reverse direction
    }

    if (mover.position.y >= screen_height-PLAYER_RADIUS || mover.position.y <= 0+PLAYER_RADIUS) {
        mover.velocity.y *= -1; // reverse direction
    }

    // wipe out acceleration
    erase_force(&mover.acceleration);
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawCircleV(mover.position, PLAYER_RADIUS, BLUE);
    
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
