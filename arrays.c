#include <stdio.h>
#define ARR_SIZE 100

typedef struct cell {
	//1 for alive, 0 for dead
	int alive;
	int aliveNeighbors;


} Cell;
typedef struct chunk {
	//Array of cells, presented in count of alive neighbors
	Cell cells[ARR_SIZE][ARR_SIZE];
	//Cell coordinates that need to be tested
	int cellsToTest[ARR_SIZE * ARR_SIZE][2];
	//Coordinate of chunk in world
	int coord[2];
} Chunk;

void testAliveNeighbors (Chunk * chunk) {
	int i, j, x, y, alive;
	for (i = 0; i < ARR_SIZE; ++i) {
		for (j = 0; j < ARR_SIZE; ++j) {
			alive = 0;
			for (x = -1; x < 2; x++) {
				for (y = -1; y < 2; y++) {
					//Temporary fix to array edges
					if (i != 0 && j != 0) {
						if (chunk->cells[i + y][j + x].alive) {
							++alive;		
						}
					}
				}
			}
			if (chunk->cells[i][j].alive)
				--alive;
			chunk->cells[i][j].aliveNeighbors = alive;			
		}
	}
}

void cellAliveState (Chunk * chunk) {
	int i, j, aliveNeighbors;
	for (i = 0; i < ARR_SIZE; ++i) {
		for (j = 0; j < ARR_SIZE; ++j) {
			aliveNeighbors = chunk->cells[i][j].aliveNeighbors;
			if(chunk->cells[i][j].alive && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
				;
			} else if ((!chunk->cells[i][j].alive) && aliveNeighbors == 3) {
				chunk->cells[i][j].alive = 1;
			} else {
				chunk->cells[i][j].alive = 0;
			}
		}
	}
}
