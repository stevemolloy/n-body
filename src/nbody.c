#include <math.h>
#include <stdio.h>

#include "raylib.h"

#define WIDTH 1000
#define HEIGHT 800

#define NUM_PARTICLES 7
#define TRAIL_LENGTH 200

typedef struct {
  long double x, y;
} Vector2d;

typedef struct {
  Vector2d position;
  Vector2d velocity;
  Vector2d acceleration;
  long double mass;
} Particle;

#define VECTOR3D_SCALE(S, V) (Vector2d) { .x=(S)*(V).x, .y=(S)*(V).y }
#define VECTOR3D_ADD(A, B)   (Vector2d) { .x=(A).x+(B).x, .y=(A).y+(B).y }
#define VECTOR3D_DIFF(A, B)  (Vector2d) { .x=(A).x-(B).x, .y=(A).y-(B).y }
#define VECTOR3D_LENGTH(V)   sqrtf((V).x*(V).x + (V).y*(V).y)

void zero_momentum(Particle *particles, size_t count) {
  long double x_momentum = 0, y_momentum = 0;
  long double total_mass = 0;

  for (size_t i=0; i<count; i++) {
    x_momentum += particles[i].velocity.x * particles[i].mass;
    y_momentum += particles[i].velocity.y * particles[i].mass;
    total_mass += particles[i].mass;
  }

  for (size_t i=0; i<count; i++) {
    particles[i].velocity.x -= x_momentum / total_mass;
    particles[i].velocity.y -= y_momentum / total_mass;
  }
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "N-body problem");
  SetTargetFPS(100);

  float part_rad[NUM_PARTICLES] = {10, 2, 2, 2, 2, 2, 2};
  Particle particles[NUM_PARTICLES] = {
    {
      .mass = 1000*1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0},
      .velocity = (Vector2d) {0, 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 - 50},
      .velocity = (Vector2d) {-140, 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 - 75},
      .velocity = (Vector2d) {-140/sqrt(1.5), 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 + 100},
      .velocity = (Vector2d) {140/sqrt(2.0), 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 + 150},
      .velocity = (Vector2d) {140/sqrtf(3.0), 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 + 200},
      .velocity = (Vector2d) {140/2.0, 0},
      .acceleration = (Vector2d) {0, 0},
    },
    {
      .mass = 1000,
      .position = (Vector2d) {GetRenderWidth()/2.0, GetRenderHeight()/2.0 + 400},
      .velocity = (Vector2d) {140/2.0/sqrtf(2.0), 0},
      .acceleration = (Vector2d) {0, 0},
    },
  };
  zero_momentum(particles, NUM_PARTICLES);

  long double acceleration_deltas[NUM_PARTICLES] = {0};
  Vector2d force_vectors[NUM_PARTICLES][NUM_PARTICLES] = {0};
  long double radii[NUM_PARTICLES][NUM_PARTICLES] = {0};

  Vector2d trail[NUM_PARTICLES][TRAIL_LENGTH] = {0};

  size_t counter = 0;
  while (!WindowShouldClose()) {
    long double dt = GetFrameTime();

    counter = (counter + 1) % TRAIL_LENGTH;
    for (size_t i=0; i<NUM_PARTICLES; i++) {
      trail[i][counter] = particles[i].position;
    }
    
    for (size_t i=0; i<NUM_PARTICLES; i++) {
      particles[i].acceleration = (Vector2d) {0};
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
      DrawCircle(particles[part].position.x, particles[part].position.y, part_rad[part], WHITE);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
