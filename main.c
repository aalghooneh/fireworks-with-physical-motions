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
#include <pthread.h>

/**
 * initializes necessary ncurses attributes
 */
extern const float lftme;
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

void updtAndDrw(particle* p, int size){
    particle_update(p, 0.01, size);
    particle_draw(p, size);
}

int main() {
    struct timespec interval = { .tv_sec = 0, .tv_nsec = 25000000 };
    size_t size = 500;

    ncurses_init();
    srand((unsigned) time(NULL));

    const int series = 5;

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

        // erase screen
        clear();

        // update all particles
        for (int i = 0; i < series; i++) {
            if (srs_prtcl[i][0].life < ((float)rand() / RAND_MAX)*0.50* lftme) {
                particle_init(srs_prtcl[i], size);
            }   
        }

        for (int i = 0; i < series; i++) {
            updtAndDrw(srs_prtcl[i], size);
        }

        // draw particles to screen
        refresh();

        // wait a bit
        struct timespec frame_interval = { 0, 16666667 }; // ~16ms per frame
        nanosleep(&frame_interval, NULL);

    }

    free(srs_prtcl);
    srs_prtcl = NULL;


    endwin();

    return 0;
}
