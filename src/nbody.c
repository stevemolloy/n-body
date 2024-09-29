#include <math.h>
#include <stdio.h>

#include "raylib.h"

#define WIDTH 800
#define HEIGHT 600

#define NUM_PARTICLES 3
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
#define VECTOR3D_ADD(A, B)   (Vector3d) { .x=(A).x+(B).x, .y=(A).y+(B).y, .z=(A).z+(B).z}
#define VECTOR3D_DIFF(A, B)  (Vector3d) { .x=(A).x-(B).x, .y=(A).y-(B).y, .z=(A).z-(B).z}
#define VECTOR3D_LENGTH(V)   sqrtf((V).x*(V).x + (V).y*(V).y + (V).z*(V).z)

int main(void) {
  InitWindow(WIDTH, HEIGHT, "N-body problem");
  SetTargetFPS(100);

  const long double mass_ratio = 3.0;
  const long double vel = 2.5;

  Particle particles[NUM_PARTICLES] = {
    {
      .mass = 10000 * mass_ratio,
      .position = (Vector3d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0, 0},
      .velocity = (Vector3d) {0, 0, 0},
      .acceleration = (Vector3d) {0, 0, 0},
    },
    {
      .mass = 10000,
      .position = (Vector3d) {GetRenderWidth()/2.0 - 200, GetRenderHeight()/2.0, 0},
      .velocity = (Vector3d) {0, vel*mass_ratio, 0},
      .acceleration = (Vector3d) {0, 0, 0},
    },
    {
      .mass = 10000,
      .position = (Vector3d) {GetRenderWidth()/2.0 + 200, GetRenderHeight()/2.0, 0},
      .velocity = (Vector3d) {0, -vel*mass_ratio, 0},
      .acceleration = (Vector3d) {0, 0, 0},
    },
  };
  long double acceleration_deltas[NUM_PARTICLES] = {0};
  Vector3d force_vectors[NUM_PARTICLES][NUM_PARTICLES] = {0};
  long double radii[NUM_PARTICLES][NUM_PARTICLES] = {0};

  Vector3d trail[NUM_PARTICLES][TRAIL_LENGTH] = {0};

  size_t counter = 0;
  while (!WindowShouldClose()) {
    long double dt = GetFrameTime();

    counter = (counter + 1) % TRAIL_LENGTH;
    for (size_t i=0; i<NUM_PARTICLES; i++) {
      trail[i][counter] = particles[i].position;
    }
    
    for (size_t i=0; i<NUM_PARTICLES; i++) {
      particles[i].acceleration = (Vector3d) {0};
      for (size_t j=0; j<NUM_PARTICLES; j++) {
        if (i == j) continue;
        force_vectors[i][j] = VECTOR3D_DIFF(particles[j].position, particles[i].position);
        radii[i][j] = VECTOR3D_LENGTH(force_vectors[i][j]);
        acceleration_deltas[i] = particles[j].mass / (radii[i][j]*radii[i][j]*radii[i][j]);
        particles[i].acceleration = VECTOR3D_ADD(particles[i].acceleration, VECTOR3D_SCALE(acceleration_deltas[i], force_vectors[i][j]));
      }
    }

    for (size_t i=0; i<NUM_PARTICLES; i++) {
      particles[i].velocity = VECTOR3D_ADD(particles[i].velocity, VECTOR3D_SCALE(dt, particles[i].acceleration));
      particles[i].position = VECTOR3D_ADD(particles[i].position, VECTOR3D_SCALE(dt, particles[i].velocity));
    }

    BeginDrawing();
    ClearBackground(BLACK);
    // DrawRectangle(0, 0, 100, 20, BLACK);
    DrawFPS(0, 0);
    for (size_t part=0; part<NUM_PARTICLES; part++) {
      for (size_t i=0; i<TRAIL_LENGTH; i++) {
        DrawCircle(trail[part][i].x, trail[part][i].y, 2, GRAY);
        DrawCircle(trail[part][i].x, trail[part][i].y, 2, GRAY);
      }
      DrawCircle(particles[part].position.x, particles[part].position.y, particles[part].mass/5000, WHITE);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
