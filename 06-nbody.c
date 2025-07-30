#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define a structure for a celestial body
typedef struct {
    double mass;
    double pos[3]; // x, y, z position
    double vel[3]; // x, y, z velocity
    double acc[3]; // x, y, z acceleration
} Body;

// Global gravitational constant
const double G = 6.67430e-11; // N(m/kg)^2

// Function to calculate forces between all bodies
void calculate_forces(Body *bodies, int num_bodies) {
    // Reset accelerations
    for (int i = 0; i < num_bodies; i++) {
        bodies[i].acc[0] = 0.0;
        bodies[i].acc[1] = 0.0;
        bodies[i].acc[2] = 0.0;
    }

    // Calculate forces
    for (int i = 0; i < num_bodies; i++) {
        for (int j = i + 1; j < num_bodies; j++) {
            double dx = bodies[j].pos[0] - bodies[i].pos[0];
            double dy = bodies[j].pos[1] - bodies[i].pos[1];
            double dz = bodies[j].pos[2] - bodies[i].pos[2];

            double dist_sq = dx * dx + dy * dy + dz * dz;
            // Add a small epsilon to prevent division by zero for coincident bodies
            double dist = sqrt(dist_sq + 1e-9); 
            double force_magnitude = (G * bodies[i].mass * bodies[j].mass) / (dist_sq + 1e-9);

            // Components of force
            double fx = force_magnitude * (dx / dist);
            double fy = force_magnitude * (dy / dist);
            double fz = force_magnitude * (dz / dist);

            // Apply force to both bodies (Newton's third law)
            bodies[i].acc[0] += fx / bodies[i].mass;
            bodies[i].acc[1] += fy / bodies[i].mass;
            bodies[i].acc[2] += fz / bodies[i].mass;

            bodies[j].acc[0] -= fx / bodies[j].mass;
            bodies[j].acc[1] -= fy / bodies[j].mass;
            bodies[j].acc[2] -= fz / bodies[j].mass;
        }
    }
}

// Function to update positions and velocities using Euler integration
void integrate(Body *bodies, int num_bodies, double dt) {
    for (int i = 0; i < num_bodies; i++) {
        // Update velocity
        bodies[i].vel[0] += bodies[i].acc[0] * dt;
        bodies[i].vel[1] += bodies[i].acc[1] * dt;
        bodies[i].vel[2] += bodies[i].acc[2] * dt;

        // Update position
        bodies[i].pos[0] += bodies[i].vel[0] * dt;
        bodies[i].pos[1] += bodies[i].vel[1] * dt;
        bodies[i].pos[2] += bodies[i].vel[2] * dt;
    }
}

int main() {
    int num_bodies = 100; // Example: 100 bodies
    double dt = 100.0;   // Time step in seconds
    double total_time = 3600.0 * 24 * 365; // Simulate for one year (example)
    int num_steps = (int)(total_time / dt);

    // Allocate memory for bodies
    Body *bodies = (Body *)malloc(num_bodies * sizeof(Body));
    if (bodies == NULL) {
        perror("Failed to allocate memory for bodies");
        return 1;
    }

    // Initialize bodies (example: random initialization)
    for (int i = 0; i < num_bodies; i++) {
        bodies[i].mass = (double)(rand() % 1000 + 1) * 1e20; // Random mass
        bodies[i].pos[0] = (double)(rand() % 1000 - 500) * 1e10; // Random position
        bodies[i].pos[1] = (double)(rand() % 1000 - 500) * 1e10;
        bodies[i].pos[2] = (double)(rand() % 1000 - 500) * 1e10;
        bodies[i].vel[0] = (double)(rand() % 100 - 50) * 1e3; // Random velocity
        bodies[i].vel[1] = (double)(rand() % 100 - 50) * 1e3;
        bodies[i].vel[2] = (double)(rand() % 100 - 50) * 1e3;
    }

    // Simulation loop
    for (int step = 0; step < num_steps; step++) {
        calculate_forces(bodies, num_bodies);
        integrate(bodies, num_bodies, dt);

        // Optional: Print or log body positions periodically
        // if (step % 1000 == 0) {
        //     printf("Step %d: Body 0 Pos: (%.2e, %.2e, %.2e)\n", step,
        //            bodies[0].pos[0], bodies[0].pos[1], bodies[0].pos[2]);
        // }
    }

    // Clean up
    free(bodies);

    return 0;
}
// Note: This code is a basic implementation of an n-body simulation using Euler integration.
// For more accurate results, consider using a more sophisticated integration method like Verlet or Runge-Kutta.
// To compile this code, use a C compiler with the C17 standard, e.g., `gcc -std=c17 06-nbody.c -o nbody`.
// Ensure to link with the math library if needed, e.g., `-lm` for math functions.
