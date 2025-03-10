/**
 * particle.h
 *
 * Author: Michael Stegeman
 * Date: 2/25/08
 *
 * Description: Forward declaration of particle structure
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



typedef struct particle_t {
    float pos[2];
    float center[2];
    float vel[2];
    float life;
    uint8_t color;
    char shape;
    bool exploded;
} particle;

void particle_init(particle *p, size_t size);

void particle_update(particle *p, float dt, size_t size);

void particle_draw(particle *p, size_t size);

float getNrmlDistVal(float mu, float sig);

#endif
