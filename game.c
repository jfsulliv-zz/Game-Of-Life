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

/*
 * Returnst he number of cells alive on the board.
 */
int num_alive(void *u, int w, int h)
{
    if(w < 0 || h < 0) return -1;
    
    unsigned (*univ)[w] = u;
    int x, y;
    int alive = 0;
    for_y for_x {
        if(univ[y][x]) alive++;
    }

    return alive;
}

/*
 * Prints the characters representing cells to the screen.
 *  Alive cells are a white block, Dead cells are spaces.
 *
 *  Flushes stdout after execution to return cursor to needed position.
 */
void display(void *u, int w, int h, int step)
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
    printf("Step: %d\n", step);
    int alive = num_alive(u, w, h);
    printf("Alive: %d", alive);
    fflush(stdout);
}

/*
 * Returns the number of cells adjacent to a given cell.
 */
int num_adjacent(void *u, int w, int h, int x, int y)
{
    if(w < 0 || h < 0) return -1;
    if(x < 0 || y < 0) return -1;
    if(x >= w || y >= w) return -1;
    unsigned (*univ)[w] = u;
    int x1, y1;
    int n = 0;
    for(y1 = y - 1; y1 <= y + 1; y1++) {
        for(x1 = x - 1; x1 <= x + 1; x1++) {
            if(univ[(y1 + h) % h][(x1 + w) %w]) n++;
        }
    }
    if(univ[y][x]) n--;
    return n;
}

/*
 * Evolves the automata's state by one step.
 *  RULES
 *   For any given cell (x,y),
 *   let (x,y) be alive IF:
 *    1) (x,y) has at least 3 alive neighbors, or
 *    2) (x,y) is alive and has at least 2 alive neighbors
 */
void evolve(void *u, int w, int h)
{
    unsigned (*univ)[w] = u;
    unsigned new[h][w];

    int x, y;
    for_y for_x {
        int n = num_adjacent(u, w, h, x, y);
        // If the cell has 3 or more adjacencies, enable.
        //  Or if the cell has 2 adjacencies and is itself enabled, enable.
        new[y][x] = (n == 3 || (n == 2 && univ[y][x])); 
    } 
    for_y for_x univ[y][x] = new[y][x];
}

/*
 * Game loop.
 *  1) Display state
 *  2) Update state
 */
void game(int w, int h)
{
    unsigned univ[h][w];
    // Random board setup
    int x, y;
    int step = 0;
    for_x for_y univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0; 
    while (1) {
        display(univ, w, h, step++);
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
