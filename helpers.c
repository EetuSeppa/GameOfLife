#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"

//Allocates memory for new chunk, increments chunkIndex
void renderChunk(int * chunkIndex, Chunk * renderedChunks[], int xCoord, int yCoord) {

	renderedChunks[*chunkIndex] = malloc(sizeof(Chunk));
	renderedChunks[*chunkIndex]->coord[0] = xCoord;
	renderedChunks[*chunkIndex]->coord[1] = yCoord;
	renderedChunks[*chunkIndex]->cellsToTestCount = 0;
	initializeZeroArray(renderedChunks[*chunkIndex]->cells);

	*chunkIndex += 1;

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

