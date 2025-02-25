#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define WIDTH  50  // Adjust as needed
#define HEIGHT 50  // Adjust as needed
#define THRESHOLD 128  // Grayscale threshold

unsigned char image[HEIGHT][WIDTH];  // Stores 1s and 0s

void read_bmp(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Error: Cannot open file.\n");
        return;
    }

    unsigned char header[54];  // BMP Header
    fread(header, sizeof(unsigned char), 54, f);  // Read BMP header

    int width = *(int*)&header[18];  // Extract image width
    int height = *(int*)&header[22]; // Extract image height

    if (width != WIDTH || height != HEIGHT) {
        printf("Image size must be %dx%d.\n", WIDTH, HEIGHT);
        fclose(f);
        return;
    }

    int row_padded = (width * 3 + 3) & (~3); // BMP rows are padded to multiples of 4 bytes
    unsigned char *data = (unsigned char *)malloc(row_padded);

    for (int y = 0; y < HEIGHT; y++) {
        fread(data, sizeof(unsigned char), row_padded, f);

        for (int x = 0; x < WIDTH; x++) {
            int index = x * 3;
            unsigned char B = data[index];
            unsigned char G = data[index + 1];
            unsigned char R = data[index + 2];

            // Convert to grayscale
            unsigned char gray = (unsigned char)(0.3 * R + 0.59 * G + 0.11 * B);

            // Apply threshold to get binary representation
            image[HEIGHT - 1 - y][x] = (gray >= THRESHOLD) ? 1 : 0;
        }
    }

    free(data);
    fclose(f);
}

// Print the matrix using ncurses at the center of the screen
void print_matrix_ncurses() {
    initscr();             // Start ncurses mode
    noecho();              // Don't echo user input
    curs_set(0);           // Hide the cursor

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);  // Get terminal size

    int start_y = (max_y - HEIGHT) / 2;  // Center Y
    int start_x = (max_x - WIDTH) / 2;   // Center X

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            mvprintw(start_y + y, start_x + x, "%c", image[y][x] ? '#' : ' '); // 1 as #, 0 as space
        }
    }

    refresh(); // Refresh the screen to display changes
    getch();   // Wait for user input
    endwin();  // Close ncurses
}

int main() {
    read_bmp("logo.bmp");
    print_matrix_ncurses();
    return 0;
}
