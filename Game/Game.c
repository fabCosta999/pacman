#include "Game.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h" 
#include "../timer/timer.h"
#include "../CAN/CAN.h"
#include <stdio.h>


#define R 24
#define C 26
#define FREQUENCY 25000000
#define PACMAN_START_X 14
#define PACMAN_START_Y 13
#define GHOST_START_X 12
#define GHOST_START_Y 13

extern Direction direction;
unsigned char map[R][C] = {		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
															{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
															{1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1},
															{1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1},
															{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
															{1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1},
															{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
															{1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
															{0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0},
															{0, 0, 0, 0, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0},
															{1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 4, 4, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1},
															{2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},
															{2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},
															{1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1},
															{0, 0, 0, 0, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0},
															{0, 0, 0, 0, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0},
															{1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
															{1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
															{1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1},
															{1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
															{1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1},
															{1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1},
															{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
															{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}	};
																
typedef struct {uint8_t x, y;} POS;
POS fifo[512];
POS pred[R][C];

																
uint8_t pacManPosX = PACMAN_START_X;
uint8_t pacManPosY = PACMAN_START_Y;
uint8_t ghostPosX = GHOST_START_X;
uint8_t ghostPosY = GHOST_START_Y;
uint16_t score = 0;
uint8_t state = 0; 
uint8_t superPills = 0;
uint8_t pillsRemaining = 240;
uint8_t lives = 1;
uint8_t time = 60;					
uint8_t ghostChasing = 1;
uint8_t lastSuperPillEaten = 80;				
uint8_t nextSuperPill = 80;
																
int horizontal_pixel(int matrixPos){
	return PADDINGL + WIDTH * matrixPos / C;
}

int vertical_pixel(int matrixPos){
	return PADDINGT + HEIGHT * matrixPos / R;
}

void clear_cell(int i, int j){
	int top = vertical_pixel(i);
	int down = top + VERTICAL_CELL_SIZE;
	int left = horizontal_pixel(j);
	int right = left + HORIZONTAL_CELL_SIZE;
	int pos;
	for (pos=top; pos<=down; ++pos)
		LCD_DrawLine(left, pos, right, pos, Black);
}

void draw_wall(int i, int j){
	int top = vertical_pixel(i) + 1;
	int down = top + VERTICAL_CELL_SIZE - 2;
	int left = horizontal_pixel(j) + 1;
	int right = left + HORIZONTAL_CELL_SIZE - 2;
	int pos;
	for (pos=top; pos<=down; ++pos)
		LCD_DrawLine(left, pos, right, pos, White);
	++pos;
}

void draw_door(int i, int j){
	int top = vertical_pixel(i) + VERTICAL_CELL_SIZE / 4;
	int down = top + VERTICAL_CELL_SIZE / 2;
	int left = horizontal_pixel(j) + 1;
	int right = left + HORIZONTAL_CELL_SIZE - 2;
	LCD_DrawLine(left, top, right, top, White);
	LCD_DrawLine(left, top, left, down, White);
	LCD_DrawLine(left, down, right, down, White);
	LCD_DrawLine(right, top, right, down, White);
}

void draw_pill(int i, int j){
	int top = vertical_pixel(i) + VERTICAL_CELL_SIZE * 2 / 5;
	int down = top + VERTICAL_CELL_SIZE / 5;
	int left = horizontal_pixel(j) + HORIZONTAL_CELL_SIZE * 2 / 5;
	int right = left + HORIZONTAL_CELL_SIZE / 5;
	int pos;
	
	for (pos=top; pos<=down; ++pos)
		LCD_DrawLine(left, pos, right, pos, Cyan);
}

void draw_super_pill(int i, int j){
	int top = vertical_pixel(i) + VERTICAL_CELL_SIZE / 5;
	int down = top + VERTICAL_CELL_SIZE * 3 / 5;
	int left = horizontal_pixel(j) + HORIZONTAL_CELL_SIZE / 5;
	int right = left + HORIZONTAL_CELL_SIZE * 3 / 5;
	int pos;
	
	for (pos=top; pos<=down; ++pos)
		LCD_DrawLine(left, pos, right, pos, Green);
}

void draw_pacman(int i, int j){
	int top = vertical_pixel(i) + VERTICAL_CELL_SIZE / 5;
	int down = top + VERTICAL_CELL_SIZE * 3 / 5;
	int left = horizontal_pixel(j) + HORIZONTAL_CELL_SIZE / 5;
	int right = left + HORIZONTAL_CELL_SIZE * 3 / 5;
	int pos;
	
	for (pos=top; pos<=down; ++pos)
		LCD_DrawLine(left, pos, right, pos, Yellow);
}

void draw_ghost(int i, int j){
	int top = vertical_pixel(i) + VERTICAL_CELL_SIZE / 5;
	int down = top + VERTICAL_CELL_SIZE * 3 / 5;
	int left = horizontal_pixel(j) + HORIZONTAL_CELL_SIZE / 5;
	int right = left + HORIZONTAL_CELL_SIZE * 3 / 5;
	int pos;
	
	for (pos=top; pos<=down; ++pos)
	LCD_DrawLine(left, pos, right, pos, ghostChasing ? Red : Blue);
}


void write_pause_message(void){	
	char msg[6] = "PAUSE";
	GUI_Text(PADDINGL + WIDTH / 2 - 20, PADDINGT + HEIGHT / 2 - 10, (uint8_t*) msg, White, Black);
}

void write_score(void){	
	char msg[5];
	sprintf(msg, "%4d", score);
	GUI_Text(PADDINGL + WIDTH / 2 - 15, PADDINGT/3, (uint8_t*) msg, White, Black);
}

void write_lives(void){	
	char msg[8];
	sprintf(msg, "vite: %d", lives);
	GUI_Text(PADDINGL, PADDINGT/3, (uint8_t*) msg, White, Black);
}

void write_time(void){	
	char msg[3];
	sprintf(msg, "%2d", time);
	GUI_Text(WIDTH - PADDINGL, PADDINGT/3, (uint8_t*) msg, White, Black);
}


void game_lost(void){
	disable_RIT();
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	disable_timer(3);
	state = 0;
	LCD_Clear(Black);
	char msg[10] = "GAME OVER";
	GUI_Text(PADDINGL + WIDTH / 2 - 36, PADDINGT + HEIGHT / 2 - 10, (uint8_t*) msg, White, Black);
}

void count_Down(void){
	unsigned char lastGhostState = ghostChasing;
	--time;
	ghostChasing = lastSuperPillEaten-time > 10;
	if (lastGhostState != ghostChasing)
		draw_ghost(ghostPosX, ghostPosY);
	if (time == nextSuperPill)
		new_super_pill();
	if (time == 0) {
		game_lost();
		return;
	}
#ifdef SIMULATOR
	write_time();
#endif
}
	
void game_won(void){
	disable_RIT();
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	disable_timer(3);
	state = 0;
	LCD_Clear(Blue);
	char msg[8] = "VICTORY";
	GUI_Text(PADDINGL + WIDTH / 2 - 28, PADDINGT + HEIGHT / 2 - 10, (uint8_t*) msg, Blue, White);
}

void add_life(){
	++lives;
	#ifdef SIMULATOR
	write_lives();
	#endif
}
	
void pacman_movement(void){
	unsigned char newPositionX, newPositionY;
	switch (direction){
		case DOWN: 
			newPositionX = pacManPosX == R-1 ? 0 : pacManPosX + 1;
			newPositionY = pacManPosY;
			break;
		case UP: 
			newPositionX = pacManPosX == 0 ? R-1 : pacManPosX - 1;
			newPositionY = pacManPosY;
			break;
		case RIGHT: 
			newPositionX = pacManPosX;
			newPositionY = pacManPosY == C-1 ? 0 : pacManPosY + 1;
			break;
		case LEFT: 
			newPositionX = pacManPosX;
			newPositionY = pacManPosY == 0 ? C-1 : pacManPosY - 1;
			break;
	}
	if (map[newPositionX][newPositionY] != 1 && map[newPositionX][newPositionY] != 4){
		if (map[newPositionX][newPositionY] == 2){
			score += 10;
			#ifdef SIMULATOR
			write_score(); 
			#endif
			if (score / 1000 > (score - 10) / 1000) add_life();
			if (--pillsRemaining == 0) {game_won();	 return;}
		}
		else if (map[newPositionX][newPositionY] == 3){
			lastSuperPillEaten = time;
			ghostChasing = 0;
			draw_ghost(ghostPosX, ghostPosY);
			score += 50;
			#ifdef SIMULATOR
			write_score();
			#endif
			if (score / 1000 > (score-50)/1000) add_life();
			if (--pillsRemaining == 0) {game_won();	 return;}
		}
		map[pacManPosX][pacManPosY] = 0;
		clear_cell(pacManPosX, pacManPosY);
		pacManPosX = newPositionX;
		pacManPosY = newPositionY;
		clear_cell(pacManPosX, pacManPosY);
		draw_pacman(pacManPosX, pacManPosY);
		map[pacManPosX][pacManPosY] = 5;
	}	
}

void check_collision(){
	if (pacManPosX==ghostPosX && pacManPosY==ghostPosY){
		if (ghostChasing){
			if (--lives == 0){
				game_lost();
				return;
			}
			#ifdef SIMULATOR
			write_lives();
			#endif
			ghostPosX = GHOST_START_X;
			ghostPosY = GHOST_START_Y;
			draw_ghost(GHOST_START_X, GHOST_START_Y);
		}
		else{
			score += 100;
			#ifdef SIMULATOR
			write_score();
			#endif
			if (score / 1000 > (score-100)/1000) add_life();
			ghostPosX = GHOST_START_X;
			ghostPosY = GHOST_START_Y;
			lastSuperPillEaten = 80;
			ghostChasing = 1;
			draw_ghost(GHOST_START_X, GHOST_START_Y);
		}
	}
}


int bfs(POS start, POS* next){
	uint8_t i, j;
	uint16_t leftIndex = 0;
	uint16_t rightIndex = 1;
	fifo[leftIndex].x = start.x;
	fifo[leftIndex].y = start.y;
	pred[start.x][start.y].x = start.x;
	pred[start.x][start.y].y = start.y;
	for (i=0; i<R; ++i){
		for (j=0; j<C; ++j){
			if (map[i][j] == 1){
				pred[i][j].x = R+1;
				pred[i][j].y = C+1;
			}
			else{
				pred[i][j].x = R;
				pred[i][j].y = C;
			}
		}
	}
	while (leftIndex != rightIndex){
		// POP
		POS pos = fifo[leftIndex]; 
		leftIndex = leftIndex == 511 ? 0 : leftIndex + 1;
		// PUSH
		POS nextPos;
		nextPos.x = pos.x == R-1 ? 0 : pos.x+1;
		nextPos.y = pos.y;
		if (pred[nextPos.x][nextPos.y].x == R) {
			pred[nextPos.x][nextPos.y] = pos;
			fifo[rightIndex] = nextPos;
			rightIndex = rightIndex == 511 ? 0 : rightIndex + 1;
		}
		nextPos.x = pos.x == 0 ? R-1 : pos.x-1;
		nextPos.y = pos.y;
		if (pred[nextPos.x][nextPos.y].x == R) {
			pred[nextPos.x][nextPos.y] = pos;
			fifo[rightIndex] = nextPos;
			rightIndex = rightIndex == 511 ? 0 : rightIndex + 1;
		}
		nextPos.x = pos.x;
		nextPos.y = pos.y == C-1 ? 0 : pos.y+1;
		if (pred[nextPos.x][nextPos.y].x == R) {
			pred[nextPos.x][nextPos.y] = pos;
			fifo[rightIndex] = nextPos;
			rightIndex = rightIndex == 511 ? 0 : rightIndex + 1;
		}
		nextPos.x = pos.x;
		nextPos.y = pos.y == 0 ? C-1 : pos.y-1;
		if (pred[nextPos.x][nextPos.y].x == R) {
			pred[nextPos.x][nextPos.y] = pos;
			fifo[rightIndex] = nextPos;
			rightIndex = rightIndex == 511 ? 0 : rightIndex + 1;
		}
		
		POS actual;
		POS prec;
		if (pred[pacManPosX][pacManPosY].x != R){
			int cnt = 0;
			actual.x = pacManPosX;
			actual.y = pacManPosY;
			prec = pred[pacManPosX][pacManPosY];
			while (prec.x!=start.x || prec.y!=start.y){
				actual = prec;
				prec = pred[prec.x][prec.y];
				++cnt;
			}
			*next = actual;
			return cnt;
		}
	}
	return -1;
}

void chase(){
	POS start, next;
	start.x = ghostPosX;
	start.y = ghostPosY;
	/*
	the ghost always find the shortest path to pacman
	*/
	bfs(start, &next);
	clear_cell(ghostPosX, ghostPosY);
	switch (map[ghostPosX][ghostPosY]){
		case 2: draw_pill(ghostPosX, ghostPosY); break;
		case 3: draw_super_pill(ghostPosX, ghostPosY); break;
		case 4: draw_door(ghostPosX, ghostPosY); break;
		default: break;
	}
	ghostPosX = next.x;
	ghostPosY = next.y;
	clear_cell(ghostPosX, ghostPosY);
	draw_ghost(ghostPosX, ghostPosY);
}


void run_away(){
	POS pos, tmp;
	uint8_t best;
	uint32_t cnt1, cnt2, cnt3, cnt4, cntBest;
	
	/*
	the ghost computes the shortest path from the cells he can reach
	to pacman, and chooses the longest one
	*/
	pos.x = ghostPosX==R-1 ? 0 : ghostPosX+1;
	pos.y = ghostPosY;
	cnt1 = map[pos.x][pos.y] == 1 ? 0 : bfs(pos, &tmp);
	pos.x = ghostPosX==0 ? R-1 : ghostPosX-1;
	pos.y = ghostPosY;
	cnt2 = map[pos.x][pos.y] == 1 ? 0 : bfs(pos, &tmp);
	pos.x = ghostPosX;
	pos.y = ghostPosY==C-1 ? 0 : ghostPosY+1;
	cnt3 = map[pos.x][pos.y] == 1 ? 0 : bfs(pos, &tmp);
	pos.x = ghostPosX;
	pos.y = ghostPosY==0 ? C-1 : ghostPosY-1;
	cnt4 = map[pos.x][pos.y] == 1 ? 0 : bfs(pos, &tmp);
	
	best = 0;
	cntBest = cnt1;
	if (cnt2 > cntBest){
		best = 1;
		cntBest = cnt2;
	}
	if (cnt3 > cntBest){
		best = 2;
		cntBest = cnt3;
	}
	if (cnt4 > cntBest)
		best = 3;
	
	switch (best){
		case 0:
			pos.x = ghostPosX==R-1 ? 0 : ghostPosX+1;
			pos.y = ghostPosY;
			break;
		case 1:
			pos.x = ghostPosX==0 ? R-1 : ghostPosX-1;
			pos.y = ghostPosY;
			break;
		case 2:
			pos.x = ghostPosX;
			pos.y = ghostPosY==C-1 ? 0 : ghostPosY+1;
			break;
		case 3:
			pos.x = ghostPosX;
			pos.y = ghostPosY==0 ? C-1 : ghostPosY-1;
			break;
	}
	clear_cell(ghostPosX, ghostPosY);
	switch (map[ghostPosX][ghostPosY]){
		case 2: draw_pill(ghostPosX, ghostPosY); break;
		case 3: draw_super_pill(ghostPosX, ghostPosY); break;
		case 4: draw_door(ghostPosX, ghostPosY); break;
		default: break;
	}
	ghostPosX = pos.x;
	ghostPosY = pos.y;
	clear_cell(ghostPosX, ghostPosY);
	draw_ghost(ghostPosX, ghostPosY);
}

void ghost_movement(void){
	check_collision();  // check if pacman reaches the ghost after his movement
	if (!state) return;
	if (ghostChasing) {
		if (rand()%100>time) // the ghost goes faster and faster as the time decresases
			chase();
	}
	else {
		if ((rand()%2) & 1) // the ghost is very slow while running away
			run_away();
	}
	check_collision(); // check if the ghost reaches pacman after his movement
}


void game_cycle(void){
	disable_RIT();
	if (state){
		pacman_movement();
		if (!state)return;
		ghost_movement();
		if (!state) return;
		#ifndef SIMULATOR
		uint32_t message = score;
		message |= ((uint32_t) lives) << 16;
		message |= ((uint32_t) time) << 24;
		CAN_TxMsg.data[0] = (message & 0xFF000000) >> 24;
		CAN_TxMsg.data[1] = (message & 0x00FF0000) >> 16;
		CAN_TxMsg.data[2] = (message & 0x0000FF00) >> 8;
		CAN_TxMsg.data[3] = message & 0x000000FF;
		CAN_TxMsg.len = 4;
		CAN_TxMsg.id = 2;
		CAN_TxMsg.format = STANDARD_FORMAT;
		CAN_TxMsg.type = DATA_FRAME;
		CAN_wrMsg (1, &CAN_TxMsg); 
		#endif
	}
	reset_RIT();
	enable_RIT();
}


void setup_map(void){
	int i, j;
	LCD_Clear(Black);
	map[pacManPosX][pacManPosY] = 5;
	for (i=0; i<R; ++i){
		for (j=0; j<C; ++j){
			switch (map[i][j]){
				case 1: draw_wall(i, j); break;
				case 2: draw_pill(i, j); break;
				case 3: draw_super_pill(i, j); break;
				case 4: draw_door(i, j); break;
				case 5: draw_pacman(i, j); break;
				default: break;
			}
		}
	}
	draw_ghost(ghostPosX, ghostPosY);
	write_pause_message();
	write_score();
	write_lives();
	write_time();
}



void pause_game(void){
	disable_RIT();
	if (state){
		disable_timer(0);
		disable_timer(2);
		state = 0;
		write_pause_message();
	}
	else{
		state = 1;
		int i;
		int j;
		for (i=11; i<=12; ++i)
			for (j=10; j<=15; ++j){
				clear_cell(i, j);
				if (ghostPosX==i && ghostPosY==j)
					draw_ghost(i, j);
		}
		if (superPills == 0){
			init_timer(2, FREQUENCY);
			srand(LPC_TIM0->TC);
			nextSuperPill = time -  (rand() % 5 + 5);
			disable_timer(0);
			init_timer(0, 0x004C4B40<<1);
			reset_timer(0);
			enable_timer(0);
		}
		enable_timer(0);
		enable_timer(2);
	}
	reset_RIT();
	enable_RIT();
}


void new_super_pill(void){
	int posX;
	int posY;
	do {
		posX = rand() % R;
		posY = rand() % C;
	} while (map[posX][posY] != 2);
	map[posX][posY] = 3;
	clear_cell(posX, posY);
	draw_super_pill(posX, posY);
	if (++superPills < 6)
		nextSuperPill = time - (rand() % 5 + 5);
	else 
		nextSuperPill = 80;
}


