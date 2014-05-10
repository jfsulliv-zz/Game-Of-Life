#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CELL(I,J) (board[size*(I) + (J)])
#define ALIVE(I,J) t[h*(I) + (J)] = 1
#define DEAD(I,J) t[h*(I) + (J)] = 0

#define for_x for(x = 0; x < w; x++)
#define for_y for(y = 0; y < h; y++)

#define DEAD_STR "\033[07m \033[m"
#define ALIVE_STR "  "

void display(void *u, int w, int h)
{
    int (*t)[w] = u;
    printf("\033[H");
    int x, y;
    for_y {
        for_x {
            printf(t[y][x] ? DEAD_STR : ALIVE_STR);
        }
        printf("\033[E");
    }
    fflush(stdout);
}

void evolve(void *u, int w, int h)
{
    unsigned (*univ)[w] = u;
    unsigned new[h][w];

    int x, y;
    for_y for_x {
        int n = 0;
        int x1, y1;
        // Count adjacencies
        for (y1 = y-1; y1 <= y+1; y1++) {
            for (x1 = x-1; x1 <= x+1; x1++) {
                if(univ[(y1 + h) % h][(x1 + w) % w]) n++;
            }
        }
        if(univ[y][x]) n--; // Remove self from adjacent
        // If the cell has 3 or more adjacencies, enable.
        //  Or if the cell has 2 adjacencies and is itself enabled, enable.
        new[y][x] = (n == 3 || (n == 2 && univ[y][x])); 
    } 
    for_y for_x univ[y][x] = new[y][x];
}

void game(int w, int h)
{
    unsigned univ[h][w];
    // Random board setup
    int x, y;
    for_x for_y univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0; 
    while (1) {
        display(univ, w, h);
        evolve(univ, w, h);
        usleep(100000);
    }
}

int main(int argc, char *argv[])
{
        int w, h = 0;
        if(argc > 1) w = atoi(argv[1]);
        if(argc > 2) h = atoi(argv[2]);
        if(w <= 0) w = 30;
        if(h <= 0) h = 30; // Defaults
        game(w,h);

        return 0;
}
