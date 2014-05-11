#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define CELL(I,J) (board[size*(I) + (J)])
#define ALIVE(I,J) t[h*(I) + (J)] = 1
#define DEAD(I,J) t[h*(I) + (J)] = 0

#define for_x for(x = 0; x < w; x++)
#define for_y for(y = 0; y < h; y++)

#define DEAD_STR "\033[07m \033[m"
#define ALIVE_STR "  "

#define BUF_SIZE 10

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
    if(w < 0 || h < 0) return;

    unsigned (*univ)[w] = u;
    unsigned new[h][w];

    int x, y;
    for_y for_x {
        int n = num_adjacent(u, w, h, x, y);
        new[y][x] = (n == 3 || (n == 2 && univ[y][x])); 
    } 
    for_y for_x univ[y][x] = new[y][x];
}

/*
 * Game loop. (w,h) give board dimensions, s gives speed, n is max steps.
 *  1) Display state
 *  2) Update state
 */
void game(int w, int h, int s, int n)
{
    unsigned univ[h][w];
    int x, y;
    int step = 0;
    int sleep_time = (6 - s)*50000;
    // Random initialization
    for_x for_y univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0; 
    while (1 || (step < n && n > 0)) {
        display(univ, w, h, step++);
        evolve(univ, w, h);
        usleep(sleep_time);
    }

    fflush(stdout);
    printf("Simulation ended after %d steps\n",step);
}

/*
 * Gets an integer input of size < BUF_SIZE from STDIN.
 *  Can give an upper and lower bound, default value, display message and default message.
 */
int get_option(int min, int max, int default_val, char *message, char *default_message)
{
    char in[BUF_SIZE];
    int val;

    printf("%s: ", message);
    fgets(in, BUF_SIZE + 1, stdin);
    val = atoi(in);
    if(!val || val < min || val > max) {
        printf("%s\n", default_message);
        val = default_val;
    }

    return val;

}

int main(int argc, char *argv[])
{
    if(argc > 1) {
        printf("USAGE: ./game");
        return 1;
    }

    int w, h, s, n;

    printf("-----CONWAY'S GAME OF LIFE-----\n");
    printf("Press CTRL + C to end game.\n");

    char *message = "Enter the board width (0 < w < 100)";
    char *default_message = "Setting to default value of 30";
    w = get_option(1, 99, 30, message, default_message);

    message = "Enter the board height (0 < h < 100)";
    h = get_option(1, 99, 30, message, default_message);

    message = "Enter the game speed (1 ... 5)";
    default_message = "Setting to default value of 3";
    s = get_option(1, 5, 3, message, default_message);

    message = "Enter the max number of steps (0 for no max)";
    default_message = "Unlimited number of steps";
    n = get_option(0, INT_MAX, 0, message, default_message); 

    printf("-----Beginning Simulation-----");
    sleep(1);
    printf("\033[2J");

    game(w,h,s,n);
    return 0;
}
