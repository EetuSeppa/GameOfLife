#include <stdio.h>
#include <string.h>
#include "helpers.h"

//Problem might be with carrying cells to test to next generation

//TODO: Getting too many neighbors

void chunkCollision (Chunk * chunk, Chunk *renderedArr[50], int * renderedIndex, int cellXCoord, int cellYCoord);



//Returns -1 if not found
int findIndex (int elementToSearch[], Chunk * arr[], int limit) {
	int i, index;	
	index = -1;

	for (i = 0; i < limit; ++i) {
		if 	(arr[i]->coord[0] == elementToSearch[0]) {
			if (arr[i]->coord[1] == elementToSearch[1]) {
				index = i;
			}
		}
	}

	return index;
}

//Need a faster way of searching, perhaps sorting first and then binary searching for x coordinate
int found (int elementToSearch[], int arr[][2], int limit) {
	int i, found;	
	found = 0;
	for (i = 0; i < limit; ++i) {
		if(arr[i][0] == elementToSearch[0]) {
			if(arr[i][1] == elementToSearch[1]) {
				found = 1;
				break;
			}
		}
	}

	return found;
}
void copyArray (Chunk * chunk, int arr2[][2]);
void printInfo (Chunk chunk) {
	int i, j;
	
	printf("%d count of tested cells\n", chunk.cellsToTestCount);
	for (i = 0; i < chunk.cellsToTestCount; ++i) {
		printf("x: %d, y: %d ", chunk.cellsToTest[i][0], chunk.cellsToTest[i][1]);
	}
	printf("\n\n");

}
void initialTestedCells (Chunk * chunk) {
	int i, x, y, cellIndex; 
	int searchArr[2];
	cellIndex = chunk->cellsToTestCount;

	for(i = 0; i < cellIndex; ++i) {
		for (x = -1; x <= 1; ++x) {
			for (y = -1; y <= 1; ++y) {
			//TODO: Take into account cells added multiple times
				searchArr[0] = chunk->cellsToTest[i][0] + x;
				searchArr[1] = chunk->cellsToTest[i][1] + y;
				if (searchArr[0] >= 0 && searchArr[0] <= ARR_SIZE - 1 && searchArr[1] >= 0 && searchArr[1] <= ARR_SIZE - 1) {
					if (!found(searchArr, chunk->cellsToTest, cellIndex)) {
						chunk->cellsToTest[chunk->cellsToTestCount][0] = chunk->cellsToTest[i][0] + x;
						chunk->cellsToTest[chunk->cellsToTestCount][1] = chunk->cellsToTest[i][1] + y;

						++chunk->cellsToTestCount;
					}
				}
			}
		}
	}
}
void testAliveNeighbors (Chunk * chunk, Chunk *renderedArr[50], int * renderedIndex) {
	int i, x, y, alive, xCoord, yCoord;
	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		alive = 0;
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];
		printf("%d, %d alive %d\n", xCoord, yCoord, chunk->cells[yCoord][xCoord].aliveNeighbors);

		if (xCoord != 0 && yCoord != 0 && xCoord != ARR_SIZE - 1 && yCoord != ARR_SIZE - 1) {
			for (x = -1; x <= 1; x++) {
				for (y = -1; y <= 1; y++) {
						if (chunk->cells[yCoord + y][xCoord + x].alive) {
							//printf("x: %d, y: %d\n", xCoord, yCoord);
							++alive;		
						}
				}
			}
			if (chunk->cells[yCoord][xCoord].alive) {
				--alive;
			}
			chunk->cells[yCoord][xCoord].aliveNeighbors = alive;			

		} else {
			chunkCollision(chunk, renderedArr, renderedIndex, xCoord, yCoord);
		}
	}
}

int cellAliveState (Chunk * chunk) {
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
		printf("%d, %d: aliveneigh %d, alivestate %d\n", xCoord, yCoord, aliveNeighbors, chunk->cells[yCoord][xCoord].alive);

		//TODO: PRINT COORDINATE AND ALIVENEIGHBORS FOR DEBUG

		if(chunk->cells[yCoord][xCoord].alive && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
			chunk->cells[yCoord][xCoord].alive = 1;
		} else if ((!chunk->cells[yCoord][xCoord].alive) && aliveNeighbors == 3) {
			chunk->cells[yCoord][xCoord].alive = 1;
		} else {
			chunk->cells[yCoord][xCoord].alive = 0;
		}
		if (chunk->cells[yCoord][xCoord].alive) {
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
		} else {
			chunk->cells[yCoord][xCoord].aliveNeighbors = 0;
		}
	}
	//printInfo(*chunk);
	copyArray(chunk, tempArray);
	chunk->cellsToTestCount = tempArrayIndex;
	return aliveCount;
}

void copyArray (Chunk * chunk, int arr[][2]) {
	memcpy(&chunk->cellsToTest, arr, sizeof(chunk->cellsToTest));
}

void chunkCollision (Chunk * chunk, Chunk *renderedArr[50], int * renderedIndex, int cellXCoord, int cellYCoord) {
	//Values needed for testing array corners
	int chunkXCoord, chunkYCoord, renderedChunkIndex, i, j, x, y, xCoordOfNeigh, yCoordOfNeigh;	

	int coordToFind[2];
	int neighborCoordsDest[3][2];
	int cellCoordsDest[3][3][2];
	int curChunkNeighborCoords[5][2];

	Chunk * curChunk;

	chunkXCoord = chunk->coord[0];
	chunkYCoord = chunk->coord[1];
	
	//Tests need to be done regardless if the cell is dead or alive
	//Update neighbor-count of this cell and neigboring cells in other arrays
	//Add cells to tested cells
	//Neighbor cell coordinates need to be added to current coord and are grouped based on chunk 

	if (cellXCoord == 0 && cellYCoord == 0) { 					//Top-left corner
		int neighborCoords[3][2] = {{0, -1}, {-1, 0}, {-1, -1}};
		int neighborCellCoord[3][3][2] = { {{0, 49}, {1, 49}, {-51, -51}}, {{49, 0}, {49, 1}, {-51, -51}}, { {49, 49}, {-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellXCoord == ARR_SIZE - 1 && cellYCoord == 0) {	//Top-right corner
		int neighborCoords[3][2] = {{0, -1}, {1, -1}, {1, 0}};
		int neighborCellCoord[3][3][2] = { {{0, 49}, {-1, 49}, {-51, -51}}, {{-49, 49}, {-51, -51}}, { {-49, 0}, {-49, 1}, {-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellXCoord == 0 && cellYCoord == ARR_SIZE - 1) {				//Bottom-left corner
		int neighborCoords[3][2] = {{0, 1}, {-1, 1}, {-1, 0}};
		int neighborCellCoord[3][3][2] = { {{0, -49}, {1, -49}, {-51, -51}}, {{49, -49}, {-51, -51}}, {{49, 0}, {49, -1}, {-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellXCoord == ARR_SIZE - 1 && cellYCoord == ARR_SIZE - 1) {	//Bottom-right corner
		int neighborCoords[3][2] = {{1, 0}, {1, 1}, {0, 1}};
		int neighborCellCoord[3][3][2] = { {{-49, 0}, {-49, -1}, {-51, -51}}, {{-49, -49}, {-51, -51}}, {{0, -49}, {-1, -49}, {-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellYCoord == 0) { 											//Top edge, mark shorter array with -2 for later processing
		int neighborCoords[3][2] = {{0, -1}, {-2, -2}};
		int neighborCellCoord[3][3][2] = {{{-1, 49}, {0, 49}, {1, 49}}, {{-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));
			

	} else if (cellXCoord == ARR_SIZE - 1) { 					//Right edge
		int neighborCoords[3][2] = {{1, 0}, {-2, -2}};
		int neighborCellCoord[3][3][2] = {{{-49, -1}, {-49, 0}, {-49, 1}}, {{-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellYCoord == ARR_SIZE - 1) { 						//Bottom edge
		int neighborCoords[3][2] = {{0, 1}, {-2, -2}};
		int neighborCellCoord[3][3][2] = {{{-1, -49}, {0, -49}, {1, -49}}, {{-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));

	} else if (cellXCoord == 0) { 								//Left edge
		int neighborCoords[3][2] = {{-1, 0}, {-2, -2}};
		int neighborCellCoord[3][3][2] = {{{49, -1}, {49, 0}, {49, 1}}, {{-51, -51}}};
		memcpy(neighborCoordsDest, neighborCoords, sizeof(neighborCoordsDest));
		memcpy(cellCoordsDest, neighborCellCoord, sizeof(cellCoordsDest));
	}

	//TODO: need to add tests for current array edges
	//Do this in a function and present different coordinates and cell indices in a struct?
	for (i = 0; i < 3; ++i) {
		if (neighborCoordsDest[i][0] == -2) {
			break;
		}

		coordToFind[0] = chunk->coord[0] + neighborCoordsDest[i][0];
		coordToFind[1] = chunk->coord[1] + neighborCoordsDest[i][1];

		if ((renderedChunkIndex	= findIndex(coordToFind, renderedArr, *renderedIndex)) == -1) {
			renderChunk(renderedIndex, renderedArr, coordToFind[0], coordToFind[1]);
			renderedChunkIndex = *renderedIndex - 1;
			printf("%d,%d coords of chunk\n", coordToFind[0], coordToFind[1]);
		}
		//Add alive states of neighbor chunk cells
		curChunk = renderedArr[renderedChunkIndex];
		for (j = 0; j < 5; ++j) {
			if (cellCoordsDest[i][j][0] == -51) {
				break;
			}

			xCoordOfNeigh = cellXCoord + cellCoordsDest[i][j][0];
			yCoordOfNeigh = cellYCoord + cellCoordsDest[i][j][1];
			if(chunk->cells[cellYCoord][cellXCoord].alive) {
				//printf("%d, %d curalive %d initial\n", xCoordOfNeigh, yCoordOfNeigh, curChunk->cells[yCoordOfNeigh][xCoordOfNeigh].aliveNeighbors);
				coordToFind[0] = xCoordOfNeigh;
				coordToFind[1] = yCoordOfNeigh;
				if (!(found(coordToFind, curChunk->cellsToTest, curChunk->cellsToTestCount))) {
					printf("got\n");
					curChunk->cellsToTest[curChunk->cellsToTestCount][0] = xCoordOfNeigh;	
					curChunk->cellsToTest[curChunk->cellsToTestCount][1] = yCoordOfNeigh;	
					curChunk->cellsToTestCount += 1;
				}
				curChunk->cells[yCoordOfNeigh][xCoordOfNeigh].aliveNeighbors += 1;	
				printf("%d, %d curalive %d\n", xCoordOfNeigh, yCoordOfNeigh, curChunk->cells[yCoordOfNeigh][xCoordOfNeigh].aliveNeighbors);
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
