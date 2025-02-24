/**
 * particle.c
 *
 * Author: Michael Stegeman
 * Date: 2/25/08
 *
 * Description: Defines particle structures and their methods
 */

#include "particle.h"
#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * initializes array of particles to same position, velocity,
 * color, and life, and random symbols
 */

const float grvty = 9.83;
const float lftme = 6.0;

// "Big" shape set
static const char bigShapes[] = { '@', '#', 'O', '%', '&'};
static const size_t bigCount = sizeof(bigShapes) / sizeof(bigShapes[0]);

// "Small" shape set
static const char smallShapes[] = { '.', ':', ';', '`', '\'' };
static const size_t smallCount = sizeof(smallShapes) / sizeof(smallShapes[0]);



float getNrmlDistVal(float mu, float sig) {
    float u1 = ((float) rand() / (float) RAND_MAX);
    float u2 = ((float) rand() / (float) RAND_MAX);
    float z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return mu + z0 * sig;
}

void particle_init(particle *p, size_t size) {
    float init_vel, col;
    size_t i;
    uint8_t init_color;
     

    // pick random upward velocity

    init_vel = (((float) rand()) / ((float) RAND_MAX) * (-LINES) - (LINES / (rand() % 4 + 1)));

    // pick random color
    init_color = (uint8_t) (rand() % 8);

    // pick random starting column
    col = rand() % COLS;

    // initialize particles
    for (i = 0; i < size; ++i) {
        p[i].pos[0] = (float) (LINES - 1); // start particles at bottom
        p[i].pos[1] = col;
        p[i].vel[0] = init_vel;
        p[i].vel[1] = 0.0;
        p[i].life = lftme;
        p[i].color = init_color;
        p[i].shape='.';
        p[i].exploded = FALSE;
    }
}

/**
 * updates particles positions, lifetimes, and velocities
 */
void particle_update(particle *p, float dt, size_t size) {
    size_t i;

    for (i = 0; i < size; ++i) {
        p[i].life -= dt;
        p[i].pos[0] += p[i].vel[0] * dt;
        p[i].pos[1] += p[i].vel[1] * dt;
        p[i].vel[0] += grvty * dt;
        
        
        if (p[i].vel[0] < 0.0 && p[i].exploded){
            p[i].vel[0] *= 0.90;
        }else {
            p[i].vel[0] *= 0.999;
        }

        p[i].vel[1] *= 0.999;

        float distance = sqrt(
            pow(p[i].pos[0] - p[i].center[0], 2) +       // line
            pow((p[i].pos[1] - p[i].center[1]) * 0.6, 2) // column, scaled down
        );
        // ...
        if (p[i].exploded) {
            if (distance < 3.0) {
                char candidate;
                // Keep picking until we find one NOT in smallShapes
                do {
                    candidate = (char)(32 + (rand() % (126 - 32 + 1)));
                } while (strchr(smallShapes, candidate) != NULL);

                p[i].shape = candidate;
            } else {
                // pick a random small shape
                p[i].shape = smallShapes[rand() % smallCount];
            }
        }

        //if (p[i].exploded){
        //    p[i].shape    = (char) (32 + (rand() % 94));
        //}
        
        if (p[i].life < 0.30 * lftme){
            p[i].shape = '.';
        }

        // check if time to explode
        if ((p[i].pos[0] <= 0.1f * (float)LINES || p[i].life < 0.80 * lftme) && !p[i].exploded) {
            p[i].exploded = TRUE;
            

            p[i].center[0] = p[i].pos[0];
            p[i].center[1] = p[i].pos[1];


            static const float max_speed = 50;

            float angle = ((float) rand() / RAND_MAX) * 2.0f * M_PI;
            float speed = ((float) rand() / RAND_MAX) * max_speed;  // e.g. max_speed = 20
            p[i].vel[0] = speed * cosf(angle);   // x-component
            p[i].vel[1] = speed * sinf(angle)/0.6;   // y-component

        
        }
    }
}

/**
 * draws particles onto display
 */
void particle_draw(particle *p, size_t size) {
    size_t i;

    // turn on color scheme
    attron(COLOR_PAIR(p[0].color));

    for (i = 0; i < size; ++i) {
        // display bright characters right after explosion
        if (p[i].life > 0.55 * lftme && p[i].life < 0.76*lftme) {
            attron(A_BOLD);
        }

        // display dim characters right before dying
        if (p[i].life < 0.10 * lftme) {
            attron(A_DIM);
        }

        float distance = sqrt(
            pow(p[i].pos[0] - p[i].center[0], 2) +       // line
            pow((p[i].pos[1] - p[i].center[1]) * 0.6, 2) // column, scaled down
        );
        if (p[i].exploded && distance > 20.5) {
            continue;
        }

        // draw the character
        mvaddch(p[i].pos[0], p[i].pos[1], p[i].shape);
    }

    // turn off color scheme
    attroff(A_BOLD);
    attroff(A_DIM);
    attroff(COLOR_PAIR(p[0].color));
}
