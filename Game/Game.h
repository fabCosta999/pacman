

#ifndef __GAME
#define __GAME

#define PADDINGL 10
#define PADDINGT 20
#define HEIGHT (320 - 2 * PADDINGT)
#define WIDTH (240 - 2 * PADDINGL)
#define VERTICAL_CELL_SIZE (HEIGHT / R)
#define HORIZONTAL_CELL_SIZE (WIDTH / C)

void setup_map(void);
void pause_game(void);
void new_super_pill(void);
void count_down(void);
void game_cycle(void);

#endif




