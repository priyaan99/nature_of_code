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
    float mass;
    Vector2 position;
    Vector2 velocity; // current velocity
    Vector2 acceleration; // velocity to achive
} Mover;

static void apply_force(Mover* mover, Vector2 force) {
    mover->acceleration = Vector2Add(mover->acceleration, Vector2Scale(force, 1/mover->mass));
}

static void erase_force(Vector2* acceleration) {
    *acceleration = VEC(0.0, 0.0);
}

static Mover mover;
static Vector2 wind_force;
static Vector2 gravity;

static void init() {
    mover.mass = 3;
    mover.position = VEC( screen_width*0.5f, screen_height*0.5f );
    mover.velocity = VEC(0.0, 0.0);
    mover.acceleration = VEC(0.0, 0.0);

    wind_force  = VEC(80.0, 0.0);
    gravity     = VEC(0.0, 200.0);
}

static void update() {
    // apply wind force
    apply_force(&mover, Vector2Scale(wind_force, GetFrameTime()));

    // apply gravity
    Vector2 scaled_gravity = Vector2Scale(gravity, mover.mass);
    apply_force(&mover, Vector2Scale(scaled_gravity, GetFrameTime()));

    // apply friction
    Vector2 friction = mover.velocity; 
    friction = Vector2Normalize(friction);
    const float c = -20.0;
    friction = Vector2Scale(friction, c);
    apply_force(&mover, Vector2Scale(friction, GetFrameTime()));

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

    DrawCircleV(mover.position, PLAYER_RADIUS, ColorAlpha(RED, 0.5));
    
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
