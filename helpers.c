#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"

//Allocates memory for new chunk, increments chunkIndex
Chunk * renderChunk(Chunk * lastChunk, int xCoord, int yCoord) {
	Chunk * curChunk;

	curChunk = malloc(sizeof(Chunk));
	lastChunk->nextChunk = curChunk;
	curChunk->coord[0] = xCoord;
	curChunk->coord[1] = yCoord;
	curChunk->cellsToTestCount = 0;
	initializeZeroArray(curChunk->cells);
	
	curChunk->nextChunk = NULL;
	
	return curChunk;
}
void initializeZeroArray (Cell arr[][ARR_SIZE]) {

	int i, j;
	
	for (i = 0; i < 50; ++i) {
		for (j = 0; j < 50; ++j) {
			arr[i][j].alive = 0;
			arr[i][j].aliveNeighbors = 0;
		}

	}
}

