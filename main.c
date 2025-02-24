/**
 * main.c
 *
 * Author: Michael Stegeman
 * Date: 2/25/08
 *
 * Description: ASCII art fireworks demo
 */

#include "particle.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

/**
 * initializes necessary ncurses attributes
 */
void ncurses_init() {
    WINDOW *window = initscr(); // initialize screen
    noecho();                   // don't display typed characters
    cbreak();                   // don't wait until RETURN is pressed
    nodelay(window, TRUE);      // don't wait for key press
    curs_set(0);                // make cursor invisible
    start_color();              // start color in terminal
    init_pair(1, 7, 0);         // White on black
    init_pair(2, 6, 0);         // Cyan on black
    init_pair(3, 5, 0);         // Magenta on black
    init_pair(4, 4, 0);         // Blue on black
    init_pair(5, 3, 0);         // Yellow on black
    init_pair(6, 2, 0);         // Green on black
    init_pair(7, 1, 0);         // Red on black
}

int main() {
    struct timespec interval = { .tv_sec = 0, .tv_nsec = 25000000 };
    size_t size = 300;

    ncurses_init();
    srand((unsigned) time(NULL));

    const int series = 8;

    particle** srs_prtcl = (particle **) calloc(sizeof(particle), series);
    for (int i =0 ; i < series ; i++) {
        srs_prtcl[i] = (particle *) calloc(sizeof(particle), size);
    }

    // initialize particle array
    for (int i = 0; i < series; i++) {
        particle_init(srs_prtcl[i], size);
    }
    // check for user input to quit program
    while (getch() != 'q') {
<<<<<<< Updated upstream
        if (p[0].life < 1.21 && p[0].life > 1.19) {
            // reinitialize second array at 3/4 life of first
            particle_init(q, size);
        } else if (p[0].life < 0.81 && p[0].life > 0.79) {
            // reinitialize third array at 1/2 life of first
            particle_init(r, size);
        } else if (p[0].life < 0.41 && p[0].life > 0.39) {
            // reinitialize fourth array at 1/4 life of first
            particle_init(s, size);
        } else if (p[0].life < 0.01) {
            // check for end of life and reinitialize array if dead
            particle_init(p, size);
        }
=======
>>>>>>> Stashed changes

        // erase screen
        clear();

        // update all particles
        for (int i = 0; i < series; i++) {
            if (srs_prtcl[i][0].life < ((float)rand() / RAND_MAX)*0.50* lftme) {
                particle_init(srs_prtcl[i], size);
            }   
        }
        for (int i = 0; i < series; i++) {
            particle_update(srs_prtcl[i], 0.01, size);
            particle_draw(srs_prtcl[i], size);
        }

        // draw particles to screen
        refresh();

        // wait a bit
        struct timespec frame_interval = { 0, 10666667 }; // ~16ms per frame
        nanosleep(&frame_interval, NULL);

    }

    free(srs_prtcl);
    srs_prtcl = NULL;


    endwin();

    return 0;
}
