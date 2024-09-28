#include <math.h>
#include <stdio.h>

#include "raylib.h"

#define WIDTH 800
#define HEIGHT 600

#define TRAIL_LENGTH 1000

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

#define VECTOR3D_SCALE(S, V) (Vector3d) { .x=(S)*(V).x, .y=(S)*(V).y, .z=(S)*(V).z }
#define VECTOR3D_ADD(A, B) (Vector3d) { .x=(A).x+(B).x, .y=(A).y+(B).y, .z=(A).z+(B).z}
#define VECTOR3D_DIFF(A, B) (Vector3d) { .x=(A).x-(B).x, .y=(A).y-(B).y, .z=(A).z-(B).z}
#define VECTOR3D_LENGTH(V) sqrtf((V).x*(V).x + (V).y*(V).y + (V).z*(V).z)

int main(void) {
  InitWindow(WIDTH, HEIGHT, "N-body problem");
  SetTargetFPS(60);

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

  Vector3d trail[2][TRAIL_LENGTH] = {0};

  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();

  size_t counter = 0;
  while (!WindowShouldClose()) {
    counter = (counter + 1) % TRAIL_LENGTH;
    trail[0][counter] = m1.position;
    trail[1][counter] = m2.position;
    double dt = GetFrameTime();
    
    Vector3d m1_force_vector = VECTOR3D_DIFF(m2.position, m1.position);
    Vector3d m2_force_vector = VECTOR3D_SCALE(-1.0, m1_force_vector);
    double r = VECTOR3D_LENGTH(m1_force_vector);
    double inverse_r = 1 / r;

    double m1_acceleration_delta = m2.mass / (r*r);
    double m2_acceleration_delta = m1.mass / (r*r);

    m1.acceleration = VECTOR3D_SCALE(m1_acceleration_delta*inverse_r, m1_force_vector);
    m2.acceleration = VECTOR3D_SCALE(m2_acceleration_delta*inverse_r, m2_force_vector);

    // m1.velocity = vector3_add(m1.velocity, vector3_scale(dt, m1.acceleration));
    m1.velocity = VECTOR3D_ADD(m1.velocity, VECTOR3D_SCALE(dt, m1.acceleration));
    // m2.velocity = vector3_add(m2.velocity, VECTOR3D_SCALE(dt, m2.acceleration));
    m2.velocity = VECTOR3D_ADD(m2.velocity, VECTOR3D_SCALE(dt, m2.acceleration));

    // m1.position = vector3_add(m1.position, VECTOR3D_SCALE(dt, m1.velocity));
    m1.position = VECTOR3D_ADD(m1.position, VECTOR3D_SCALE(dt, m1.velocity));
    // m2.position = vector3_add(m2.position, VECTOR3D_SCALE(dt, m2.velocity));
    m2.position = VECTOR3D_ADD(m2.position, VECTOR3D_SCALE(dt, m2.velocity));

    // DrawLine(m2.position.x, m2.position.y, m2.position.x+(50*m2_force_vector.x) , m2.position.y+(50*m2_force_vector.y), WHITE);

    BeginDrawing();
    ClearBackground(BLACK);
    // DrawRectangle(0, 0, 100, 20, BLACK);
    DrawFPS(0, 0);
    for (size_t i=0; i<TRAIL_LENGTH; i++) {
      DrawCircle(trail[0][i].x, trail[0][i].y, m1.radius, WHITE);
      DrawCircle(trail[1][i].x, trail[1][i].y, m2.radius, WHITE);
    }
    DrawCircle(m1.position.x, m1.position.y, m1.radius, WHITE);
    DrawCircle(m2.position.x, m2.position.y, m2.radius, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
