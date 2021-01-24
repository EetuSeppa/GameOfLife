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
	drawnVersion->x = chunk->coord[0];
	drawnVersion->y = chunk->coord[1];
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
	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];
		aliveNeighbors = chunk->cells[yCoord][xCoord].aliveNeighbors;

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
	// printInfo(*chunk);
	drawnVersion->x = chunk->coord[0];	
	drawnVersion->y = chunk->coord[1];	
	copyArray(chunk, tempArray);
	chunk->cellsToTestCount = tempArrayIndex;
	return aliveCount;
}

void copyArray (Chunk * chunk, int arr[][2]) {
	memcpy(&chunk->cellsToTest, arr, sizeof(chunk->cellsToTest));
}

void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount) {

	if (cellXCoord == 0 && cellYCoord == 0) { 					//Top-left corner
		addAliveStatesToNeighbors(collisionNeighbor->upLeft.neighChunkCoords, 
		collisionNeighbor->upLeft.neighCellsInNeighChunks,
		collisionNeighbor->upLeft.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);
	} else if (cellXCoord == ARR_SIZE - 1 && cellYCoord == 0) {	//Top-right corner
		addAliveStatesToNeighbors(collisionNeighbor->upRight.neighChunkCoords, 
		collisionNeighbor->upRight.neighCellsInNeighChunks,
		collisionNeighbor->upRight.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	} else if (cellXCoord == 0 && cellYCoord == ARR_SIZE - 1) {				//Bottom-left corner
		addAliveStatesToNeighbors(collisionNeighbor->downLeft.neighChunkCoords, 
		collisionNeighbor->downLeft.neighCellsInNeighChunks,
		collisionNeighbor->downLeft.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	} else if (cellXCoord == ARR_SIZE - 1 && cellYCoord == ARR_SIZE - 1) {	//Bottom-right corner
		addAliveStatesToNeighbors(collisionNeighbor->downRight.neighChunkCoords, 
		collisionNeighbor->downRight.neighCellsInNeighChunks,
		collisionNeighbor->downRight.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	} else if (cellYCoord == 0) { 											//Top edge, mark shorter array with -2 for later processing
		addAliveStatesToNeighbors(collisionNeighbor->up.neighChunkCoords, 
		collisionNeighbor->up.neighCellsInNeighChunks,
		collisionNeighbor->up.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);
			

	} else if (cellXCoord == ARR_SIZE - 1) { 					//Right edge
		addAliveStatesToNeighbors(collisionNeighbor->right.neighChunkCoords, 
		collisionNeighbor->right.neighCellsInNeighChunks,
		collisionNeighbor->right.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	} else if (cellYCoord == ARR_SIZE - 1) { 						//Bottom edge
		addAliveStatesToNeighbors(collisionNeighbor->down.neighChunkCoords, 
		collisionNeighbor->down.neighCellsInNeighChunks,
		collisionNeighbor->down.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	} else if (cellXCoord == 0) { 								//Left edge
		addAliveStatesToNeighbors(collisionNeighbor->left.neighChunkCoords, 
		collisionNeighbor->left.neighCellsInNeighChunks,
		collisionNeighbor->left.neighChunkCount,
		chunk, cellXCoord, cellYCoord, firstChunk, lastChunk, renderedChunkCount);

	}
}

void addAliveStatesToNeighbors (int neighborChunkCoords[3][2], int cellCoordsInNeighbors[3][3][2], 
		int neighborChunkCount, Chunk * chunk, int cellXCoord, int cellYCoord, 
		Chunk * firstChunk, Chunk ** lastChunk, int *renderedChunkCount) 
{
	int chunkXCoord, chunkYCoord, renderedChunkIndex, i, j, x, y, xCoordOfNeigh, yCoordOfNeigh;	

	int coordToFind[2];
	int neighborCoordsDest[3][2];
	int cellCoordsDest[3][3][2];
	int curChunkNeighborCoords[5][2];

	Chunk * curChunk;

	chunkXCoord = chunk->coord[0];
	chunkYCoord = chunk->coord[1];
	for (i = 0; i < neighborChunkCount; ++i) {
		coordToFind[0] = chunk->coord[0] + neighborChunkCoords[i][0];
		coordToFind[1] = chunk->coord[1] + neighborChunkCoords[i][1];

		if ((curChunk = findIndex(coordToFind, firstChunk)) == NULL) {
			curChunk = renderChunk(lastChunk, coordToFind[0], coordToFind[1]);
			*renderedChunkCount += 1;
		}
		//Add alive states of neighbor chunk cells
		for (j = 0; j < 5; ++j) {
			if (cellCoordsInNeighbors[i][j][0] == -ARR_SIZE - 2) {
				break;
			}
			xCoordOfNeigh = cellXCoord + cellCoordsInNeighbors[i][j][0];
			yCoordOfNeigh = cellYCoord + cellCoordsInNeighbors[i][j][1];
			if(chunk->cells[cellYCoord][cellXCoord].alive) {
				coordToFind[0] = xCoordOfNeigh;
				coordToFind[1] = yCoordOfNeigh;
				if (!(found(coordToFind, curChunk->cellsToTest, curChunk->cellsToTestCount))) {
					curChunk->cellsToTest[curChunk->cellsToTestCount][0] = xCoordOfNeigh;	
					curChunk->cellsToTest[curChunk->cellsToTestCount][1] = yCoordOfNeigh;	
					curChunk->cellsToTestCount += 1;
				}
				curChunk->cells[yCoordOfNeigh][xCoordOfNeigh].aliveNeighbors += 1;	
			}
		}
	}
	//Add alive states of current chunk cells
	for (x = -1; x <= 1; ++x) {
		for (y = -1; y <= 1; ++y) {
			if (cellXCoord + x >= 0 && cellXCoord + x <= ARR_SIZE - 1 && 
				cellYCoord + y >= 0 && cellYCoord + y <= ARR_SIZE - 1) {

				if (chunk->cells[cellYCoord + y][cellXCoord + x].alive) {
					chunk->cells[cellYCoord][cellXCoord].aliveNeighbors += 1;
				}
			}
		}
	}
	if (chunk->cells[cellYCoord][cellXCoord].alive) {
		chunk->cells[cellYCoord][cellXCoord].aliveNeighbors -= 1;
	}
}
