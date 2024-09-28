#include <math.h>

#include "raylib.h"

#define WIDTH 800
#define HEIGHT 600

typedef struct {
  double x, y, z;
} Vector3d;

typedef struct {
  Vector3d position;
  Vector3d velocity;
  Vector3d acceleration;
  double radius;
  double mass;
} Particle;

Vector3d vector3_scale(double scale, Vector3d vect) {
  return (Vector3d) {
    .x = vect.x * scale,
    .y = vect.y * scale,
    .z = vect.z * scale
  };
}

Vector3d vector3_add(Vector3d a, Vector3d b) {
  return (Vector3d) {
    .x = a.x + b.x,
    .y = a.y + b.y,
    .z = a.z + b.z
  };
}

Vector3d vector3_diff(Vector3d a, Vector3d b) {
  return (Vector3d) {
    .x = a.x - b.x,
    .y = a.y - b.y,
    .z = a.z - b.z
  };
}

inline double vector3_length(Vector3d vect) {
  return sqrtf(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "N-body problem");
  // SetTargetFPS(60);

  const double mass_ratio = 2.0;
  const double vel = 2.0;

  Particle m1 = {
    .mass = 10000 * mass_ratio,
    .radius = 2,
    .position = (Vector3d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0, 0},
    .velocity = (Vector3d) {0, -vel, 0},
    .acceleration = (Vector3d) {0, 0, 0},
  };

  Particle m2 = {
    .mass = 10000,
    .radius = 2,
    .position = (Vector3d) {GetRenderWidth()/2.0 - 200, GetRenderHeight()/2.0, 0},
    .velocity = (Vector3d) {0, vel*mass_ratio, 0},
    .acceleration = (Vector3d) {0, 0, 0},
  };

  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();

  while (!WindowShouldClose()) {
    double dt = GetFrameTime();
    m1.mass *= 1.000025;
    m2.mass /= 1.000025;
    
    Vector3d m1_force_vector = vector3_diff(m2.position, m1.position);
    Vector3d m2_force_vector = vector3_scale(-1.0, m1_force_vector);
    double r = vector3_length(m1_force_vector);
    double inverse_r = 1 / r;

    double m1_acceleration_delta = m2.mass / (r*r);
    double m2_acceleration_delta = m1.mass / (r*r);

    m1.acceleration = vector3_scale(m1_acceleration_delta*inverse_r, m1_force_vector);
    m2.acceleration = vector3_scale(m2_acceleration_delta*inverse_r, m2_force_vector);

    m1.velocity = vector3_add(m1.velocity, vector3_scale(dt, m1.acceleration));
    m2.velocity = vector3_add(m2.velocity, vector3_scale(dt, m2.acceleration));

    m1.position = vector3_add(m1.position, vector3_scale(dt, m1.velocity));
    m2.position = vector3_add(m2.position, vector3_scale(dt, m2.velocity));

    // DrawLine(m2.position.x, m2.position.y, m2.position.x+(50*m2_force_vector.x) , m2.position.y+(50*m2_force_vector.y), WHITE);

    BeginDrawing();
    // ClearBackground(BLACK);
    DrawRectangle(0, 0, 100, 20, BLACK);
    DrawFPS(0, 0);
    DrawCircle(m1.position.x, m1.position.y, m1.radius, WHITE);
    DrawCircle(m2.position.x, m2.position.y, m2.radius, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
