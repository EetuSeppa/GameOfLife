#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "arrays.h"


NeighChunkStruct * collisionNeighbor;

void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount);

void copyArray (Chunk * chunk, int arr2[][2]);

void initGlobalVariable (void) {
	initializeNeighborCoords(&collisionNeighbor);
}

void printInfo (Chunk chunk) {
	int i, j;
	
	printf("%d count of tested cells\n", chunk.cellsToTestCount);
	for (i = 0; i < chunk.cellsToTestCount; ++i) {
		printf("\tx: %d, y: %d \talive: %d\n", chunk.cellsToTest[i][0], chunk.cellsToTest[i][1], 
											chunk.cells[chunk.cellsToTest[i][0]][chunk.cellsToTest[i][1]].alive);
	}
	printf("\n\n");

}
void initialTestedCells (Chunk * chunk, DrawnChunk *drawnVersion) {
	int i, x, y, cellIndex; 
	int searchArr[2];
	cellIndex = chunk->cellsToTestCount;

	for(i = 0; i < cellIndex; ++i) {
		for (x = -1; x <= 1; ++x) {
			for (y = -1; y <= 1; ++y) {
				searchArr[0] = chunk->cellsToTest[i][0] + x;
				searchArr[1] = chunk->cellsToTest[i][1] + y;
				if (searchArr[0] >= 0 && searchArr[0] <= ARR_SIZE - 1 && searchArr[1] >= 0 && searchArr[1] <= ARR_SIZE - 1) {
					if (chunk->cells[searchArr[1]][searchArr[0]].alive) {
						drawnVersion->cellCoords[drawnVersion->cellCount].x = searchArr[0];
						drawnVersion->cellCoords[drawnVersion->cellCount].y = searchArr[1];
						drawnVersion->cellCount += 1;
					}
					if (!found(searchArr, chunk->cellsToTest, chunk->cellsToTestCount)) {
						chunk->cellsToTest[chunk->cellsToTestCount][0] = chunk->cellsToTest[i][0] + x;
						chunk->cellsToTest[chunk->cellsToTestCount][1] = chunk->cellsToTest[i][1] + y;

						++chunk->cellsToTestCount;
					}
					
				}
			}
		}
	}
	drawnVersion->x = chunk->x;
	drawnVersion->y = chunk->y;
}
void testAliveNeighbors (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int *renderedChunkCount) {
	int i, x, y, alive, xCoord, yCoord;

	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		alive = 0;
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];

		if (xCoord != 0 && yCoord != 0 && xCoord != ARR_SIZE - 1 && yCoord != ARR_SIZE - 1) {
			for (x = -1; x <= 1; x++) {
				for (y = -1; y <= 1; y++) {
						if (chunk->cells[yCoord + y][xCoord + x].alive) {
							++alive;		
						}
				}
			}
			if (chunk->cells[yCoord][xCoord].alive) {
				--alive;
			}
			chunk->cells[yCoord][xCoord].aliveNeighbors = alive;			

		} else {
			chunkCollision(chunk, lastChunk, firstChunk, xCoord, yCoord, renderedChunkCount);
			
		}
	}
}

int cellAliveState (Chunk * chunk, DrawnChunk * drawnVersion) {
	int i, x, y, aliveNeighbors, tempArrayIndex, xCoord, yCoord, aliveCount;
	int elementToSearch[2];
	//Do smarter memory allocation
	int tempArray[ARR_SIZE * ARR_SIZE][2];
	tempArrayIndex = 0;

	aliveCount = 0;
//	printf("Chunk %d, %d\n", chunk->x, chunk->y);
	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];
		aliveNeighbors = chunk->cells[yCoord][xCoord].aliveNeighbors;
	//	printf("\t%d\t%d, %d alive: %d\n", i, xCoord, yCoord, aliveNeighbors);

		if(chunk->cells[yCoord][xCoord].alive && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
			chunk->cells[yCoord][xCoord].alive = 1;
		} else if ((!chunk->cells[yCoord][xCoord].alive) && aliveNeighbors == 3) {
			chunk->cells[yCoord][xCoord].alive = 1;
		} else {
			chunk->cells[yCoord][xCoord].alive = 0;
		}
		chunk->cells[yCoord][xCoord].aliveNeighbors = 0;
		if (chunk->cells[yCoord][xCoord].alive) {
			drawnVersion->cellCoords[drawnVersion->cellCount].x = xCoord;
			drawnVersion->cellCoords[drawnVersion->cellCount].y = yCoord;
			drawnVersion->cellCount += 1;

			++aliveCount;
			for (x = -1; x <= 1; x++) {
				for (y = -1; y <= 1; ++y) {
					elementToSearch[0] = xCoord + x;				
					elementToSearch[1] = yCoord + y;				
					if (elementToSearch[0] >= 0 && elementToSearch[0] <= ARR_SIZE - 1 && elementToSearch[1] >= 0 && elementToSearch[1] <= ARR_SIZE - 1) {
						if (!found(elementToSearch, tempArray, tempArrayIndex)) {
							tempArray[tempArrayIndex][0] = xCoord + x;			
							tempArray[tempArrayIndex][1] = yCoord + y;			
							++tempArrayIndex;
						}
					}
				}
			}
		}
	}
	//printInfo(*chunk);
	drawnVersion->x = chunk->x;	
	drawnVersion->y = chunk->y;	
	copyArray(chunk, tempArray);
	chunk->cellsToTestCount = tempArrayIndex;
	return aliveCount;
}

void copyArray (Chunk * chunk, int arr[][2]) {
	memcpy(&chunk->cellsToTest, arr, sizeof(chunk->cellsToTest));
}

void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount) {
	int curChunkCoordX, curChunkCoordY, curIndexX, curIndexY;
	int i, j;
	int searchElement[2];
	int coordToFind[2];
	int diffChunkBool; //1 if cell is in neighbor, 0 otherwise
	int cellTestCount;
	Chunk *neighborChunk;

	for (i = -1; i <= 1; ++i) {
		for (j = -1; j <= 1; ++j) {
			diffChunkBool = 0;
			searchElement[0] = chunk->x;
			searchElement[1] = chunk->y;
			
			curIndexX = cellXCoord + i;
			curIndexY = cellYCoord + j;

			//If x index is outside of range, find correct chunk and edit index
			if (curIndexX < 0) {
				searchElement[0] -= 1;	
				diffChunkBool = 1;
				curIndexX = ARR_SIZE - 1;
			} else if (curIndexX > ARR_SIZE - 1) {
				searchElement[0] += 1;	
				diffChunkBool = 1;
				curIndexX = 0;
			}


			//Same logic for y index
			if (curIndexY < 0) {
				searchElement[1] -= 1;	
				diffChunkBool = 1;
				curIndexY = ARR_SIZE - 1;
			} else if (curIndexY > ARR_SIZE - 1) {
				searchElement[1] += 1;	
				diffChunkBool = 1;
				curIndexY = 0;
			}


			if (diffChunkBool) {
				if (chunk->cells[cellYCoord][cellXCoord].alive) {
					
					//If neighbor chunk is not in memory, call function to allocate and insert to list
					if ((neighborChunk = findIndex(searchElement, firstChunk)) == NULL) {
						neighborChunk = renderChunk(lastChunk, searchElement[0], searchElement[1]);
						*renderedChunkCount += 1;
					}

					
					coordToFind[0] = curIndexX;
					coordToFind[1] = curIndexY;

					if (!(found(coordToFind, neighborChunk->cellsToTest, neighborChunk->cellsToTestCount))) {
						cellTestCount = neighborChunk->cellsToTestCount;
						neighborChunk->cellsToTest[cellTestCount][0] = curIndexX;
						neighborChunk->cellsToTest[cellTestCount][1] = curIndexY;
						neighborChunk->cellsToTestCount += 1;
					}

					neighborChunk->cells[curIndexY][curIndexX].aliveNeighbors += 1;

				}
			} else {
				if (chunk->cells[curIndexY][curIndexX].alive) {
					chunk->cells[cellYCoord][cellXCoord].aliveNeighbors += 1;
				}

			}
		}
	}
	if (chunk->cells[cellYCoord][cellXCoord].alive) {
		chunk->cells[cellYCoord][cellXCoord].aliveNeighbors -= 1;
	}

}
