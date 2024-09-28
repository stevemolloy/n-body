#include <math.h>
#include <stdio.h>

#include "raylib.h"

#define WIDTH 800
#define HEIGHT 600

#define TRAIL_LENGTH 500

typedef struct {
  long double x, y, z;
} Vector3d;

typedef struct {
  Vector3d position;
  Vector3d velocity;
  Vector3d acceleration;
  long double mass;
} Particle;

#define VECTOR3D_SCALE(S, V) (Vector3d) { .x=(S)*(V).x, .y=(S)*(V).y, .z=(S)*(V).z }
#define VECTOR3D_ADD(A, B) (Vector3d) { .x=(A).x+(B).x, .y=(A).y+(B).y, .z=(A).z+(B).z}
#define VECTOR3D_DIFF(A, B) (Vector3d) { .x=(A).x-(B).x, .y=(A).y-(B).y, .z=(A).z-(B).z}
#define VECTOR3D_LENGTH(V) sqrtf((V).x*(V).x + (V).y*(V).y + (V).z*(V).z)

int main(void) {
  InitWindow(WIDTH, HEIGHT, "N-body problem");
  SetTargetFPS(100);

  const long double mass_ratio = 3.0;
  const long double vel = 2.0;

  Particle particles[2] = {
    {
      .mass = 10000 * mass_ratio,
      .position = (Vector3d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0, 0},
      .velocity = (Vector3d) {0, -vel, 0},
      .acceleration = (Vector3d) {0, 0, 0},
    },
    {
      .mass = 10000,
      .position = (Vector3d) {GetRenderWidth()/2.0 - 200, GetRenderHeight()/2.0, 0},
      .velocity = (Vector3d) {0, vel*mass_ratio, 0},
      .acceleration = (Vector3d) {0, 0, 0},
    }
  };

  Vector3d trail[2][TRAIL_LENGTH] = {0};

  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();

  size_t counter = 0;
  while (!WindowShouldClose()) {
    counter = (counter + 1) % TRAIL_LENGTH;
    trail[0][counter] = particles[0].position;
    trail[1][counter] = particles[1].position;
    long double dt = GetFrameTime();
    
    Vector3d m1_force_vector = VECTOR3D_DIFF(particles[1].position, particles[0].position);
    Vector3d m2_force_vector = VECTOR3D_SCALE(-1.0, m1_force_vector);
    long double r = VECTOR3D_LENGTH(m1_force_vector);

    long double m1_acceleration_delta = particles[1].mass / (r*r*r);
    long double m2_acceleration_delta = particles[0].mass / (r*r*r);

    particles[0].acceleration = VECTOR3D_SCALE(m1_acceleration_delta, m1_force_vector);
    particles[1].acceleration = VECTOR3D_SCALE(m2_acceleration_delta, m2_force_vector);

    particles[0].velocity = VECTOR3D_ADD(particles[0].velocity, VECTOR3D_SCALE(dt, particles[0].acceleration));
    particles[1].velocity = VECTOR3D_ADD(particles[1].velocity, VECTOR3D_SCALE(dt, particles[1].acceleration));

    particles[0].position = VECTOR3D_ADD(particles[0].position, VECTOR3D_SCALE(dt, particles[0].velocity));
    particles[1].position = VECTOR3D_ADD(particles[1].position, VECTOR3D_SCALE(dt, particles[1].velocity));

    BeginDrawing();
    ClearBackground(BLACK);
    // DrawRectangle(0, 0, 100, 20, BLACK);
    DrawFPS(0, 0);
    for (size_t i=0; i<TRAIL_LENGTH; i++) {
      DrawCircle(trail[0][i].x, trail[0][i].y, 2, GRAY);
      DrawCircle(trail[1][i].x, trail[1][i].y, 2, GRAY);
    }
    DrawCircle(particles[0].position.x, particles[0].position.y, particles[0].mass/5000, WHITE);
    DrawCircle(particles[1].position.x, particles[1].position.y, particles[1].mass/5000, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
