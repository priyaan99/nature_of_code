#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define VEC(x, y) ((Vector2) {x, y})

static const int screen_width = 320; // 256 * 1.25
static const int screen_height= 256;

/// SOH CAH TOA
/// sin(angle) => y (opposite) / length  (hypotenuse);
/// cos(angle) => x (adjacent) / length  (hypotenuse);
/// tan(angle) => y (opposite) / x (adjacent);

/// Polar cordinate = (radius, angle) [angle is in radian]
/// Cartisian cordinate = (x, y);
///     
///     2 * PI == 360 degree , or
///     PI == 180 degree
/// 
/// Polar to Cartisian
/// y = radius * sin(angle);
/// x = radius * cos(angle);
///
/// "Think in terms of Polar coordinate and work in terms of Cartesian"

typedef struct {
    Vector2 origin;
    Vector2 end;
    float radius;
    float angle; // radian
    float angular_velocity;
    float angular_acceleration;
} Pendulum;

static Pendulum pendulum;

static inline Vector2 polar_to_cartisian(float radius, float angle) {
    Vector2 result;
    result.x = radius * cosf(angle);
    result.y = radius * sinf(angle);
    return result;
}

static void init() {
    pendulum.origin = VEC( screen_width *0.5 , screen_height * 0.5 );
    pendulum.radius = 40;
    pendulum.angle  = 1;
    pendulum.end = polar_to_cartisian(pendulum.radius, pendulum.angle);
    pendulum.end = Vector2Add(pendulum.origin, pendulum.end);

    pendulum.angular_velocity       = 0.0;
    pendulum.angular_acceleration   = 0.0;
}

static void update() {
    pendulum.angular_acceleration = 2.0;
    pendulum.angular_velocity += pendulum.angular_acceleration * GetFrameTime();
    // pendulum.angular_velocity = pendulum.angular_velocity > 5.0 ? 5.0 : pendulum.angular_velocity;

    pendulum.angle += pendulum.angular_velocity * GetFrameTime();
    pendulum.angle = Wrap(pendulum.angle, 0.0, 2*PI);

    pendulum.end = polar_to_cartisian(pendulum.radius, pendulum.angle);
    pendulum.end = Vector2Add(pendulum.origin, pendulum.end);
}

static void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawCircleV(pendulum.origin, 5, RED);
    DrawLineEx(pendulum.origin, pendulum.end, 3, RED);
    DrawCircleV(pendulum.end, 10, RED);
    EndDrawing();
}

int main() {
    InitWindow(screen_width, screen_height, "Pendulum");
    SetTargetFPS(60);

    init();

    while (!WindowShouldClose()) {
        update();
        draw();
    }

    CloseWindow();
}
