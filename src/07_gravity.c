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
    float radius;
    Vector2 position;
    Vector2 velocity; // current velocity
    Vector2 acceleration; // velocity to achive
} Entity;

static void apply_force(Vector2* acceleration, Vector2 force) {
    *acceleration = Vector2Add(*acceleration, force);
}

static void erase_force(Vector2* acceleration) {
    *acceleration = VEC(0.0, 0.0);
}

static Entity mover;
static Entity earth;

static void init() {
    mover.radius = 5;
    mover.position = VEC( screen_width*0.5f, mover.radius+10 );
    mover.velocity = VEC(0.8, 0.0);
    mover.acceleration = VEC(0.0, 0.0);
    mover.mass = 10;

    earth.position = VEC( screen_width*0.5f, screen_height*0.5f );
    earth.velocity = VEC(0.0, 0.0);
    earth.acceleration = VEC(0.0, 0.0);
    earth.mass = 1000;
    earth.radius = 20;
}

static void update() {
    const float GRAVITY = 30.0f; // teak the value to see difference

    /// Gravity = (G * m1 * m2 * direction) / (d * d)

    float gravity = ( GRAVITY * mover.mass * earth.mass ) / Vector2DistanceSqr(mover.position, earth.position);
    Vector2 gravity_vec = Vector2Scale(Vector2Normalize(Vector2Subtract(earth.position, mover.position)), gravity);

    apply_force(&mover.acceleration, Vector2Scale(gravity_vec, GetFrameTime()));

    mover.velocity = Vector2Add(mover.velocity, Vector2Scale(mover.acceleration, GetFrameTime()));
    mover.position = Vector2Add(mover.position, mover.velocity);

    // wipe out acceleration
    erase_force(&mover.acceleration);
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawCircleV(mover.position, mover.radius, ColorAlpha(RED, 0.7));
    DrawCircleV(earth.position, earth.radius, ColorAlpha(BLUE, 0.5));
    
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
