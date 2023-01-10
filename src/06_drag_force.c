#include "raylib.h"
#include "raymath.h"

/// A Force is a vector that causes an object with mass to accelerate
/// * F (force) = M (mass) * A (acceleration)
/// * A = F / M
/// * if M == 1, then A = F

#define VEC(x, y) ((Vector2) {x, y})

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
static Vector2 drag_force;
static Vector2 gravity;

static void init() {
    mover.mass = 3;
    mover.position = VEC( screen_width*0.5f, PLAYER_RADIUS );
    mover.velocity = VEC(0.0, 0.0);
    mover.acceleration = VEC(0.0, 0.0);

    drag_force = VEC(0.0, 0.0);
    gravity     = VEC(0.0, 200.0);
}

static void update() {
    // apply drag force
    if (IsKeyDown(KEY_SPACE)) {
        drag_force = Vector2Normalize(mover.velocity);
        const float c = -30.0;
        float speed  = Vector2Length(mover.velocity);
        drag_force = Vector2Scale(drag_force, c*speed*speed);

        apply_force(&mover, drag_force);
    }

    // apply gravity
    Vector2 scaled_gravity = Vector2Scale(gravity, mover.mass);
    apply_force(&mover, Vector2Scale(scaled_gravity, GetFrameTime()));

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

    DrawText("Press Space to apply Drag force", 20, 20, 16, ColorAlpha(BLUE, 0.8));
    DrawCircleV(mover.position, PLAYER_RADIUS, ColorAlpha(RED, 0.5));
    
    EndDrawing();
}

int main() {
    InitWindow(screen_width, screen_height, "Drag Force");
    SetTargetFPS(60);

    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();

}
