#include <stdio.h>
#include <string.h>
#define ARR_SIZE 50

//TODO: Is there a nice way to find if cell is already rendered?
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
	int cellsToTestCount;
	//Coordinate of chunk in world
	int coord[2];
} Chunk;

//Need a faster way of searching, perhaps sorting first and then binary searching for x coordinate
int found (int elementToSearch[], int arr[][2], int limit) {
	int i, found;	
	found = 0;
	for (i = 0; i < limit; ++i) {
		if(arr[i][0] == elementToSearch[0]) {
			if(arr[i][1] == elementToSearch[1]) {
				found = 1;
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
	printf("Initial...\n");
	printInfo(*chunk);
	int i, x, y, cellIndex; 
	int searchArr[2];
	cellIndex = chunk->cellsToTestCount;

	for(i = 0; i < cellIndex; ++i) {
		for (x = -1; x <= 1; ++x) {
			for (y = -1; y <= 1; ++y) {
			//TODO: Take into account cells added multiple times
				searchArr[0] = chunk->cellsToTest[i][0] + x;
				searchArr[1] = chunk->cellsToTest[i][1] + y;
				if (!found(searchArr, chunk->cellsToTest, cellIndex)) {
					chunk->cellsToTest[chunk->cellsToTestCount][0] = chunk->cellsToTest[i][0] + x;
					chunk->cellsToTest[chunk->cellsToTestCount][1] = chunk->cellsToTest[i][1] + y;

					++chunk->cellsToTestCount;
				}
			}
		}
	}
}
void testAliveNeighbors (Chunk * chunk) {
	int i, x, y, alive, xCoord, yCoord;
	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		alive = 0;
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];
		for (x = -1; x <= 1; x++) {
			for (y = -1; y <= 1; y++) {
				//Temporary fix to array edges
				if (xCoord != 0 && yCoord != 0) {
					if (chunk->cells[yCoord + y][xCoord + x].alive) {
						++alive;		
					}
				}
			}
		}
		if (chunk->cells[yCoord][xCoord].alive) {
			--alive;
		}
		chunk->cells[yCoord][xCoord].aliveNeighbors = alive;			
	}
}

void cellAliveState (Chunk * chunk) {
	int i, x, y, aliveNeighbors, tempArrayIndex, xCoord, yCoord;
	int elementToSearch[2];
	//Do smarter memory allocation
	int tempArray[ARR_SIZE * ARR_SIZE][2];
	tempArrayIndex = 0;

	for (i = 0; i < chunk->cellsToTestCount; ++i) {
		xCoord = chunk->cellsToTest[i][0];
		yCoord = chunk->cellsToTest[i][1];
		aliveNeighbors = chunk->cells[yCoord][xCoord].aliveNeighbors;

		if(chunk->cells[yCoord][xCoord].alive && (aliveNeighbors == 2 || aliveNeighbors == 3)) {
			;
		} else if ((!chunk->cells[yCoord][xCoord].alive) && aliveNeighbors == 3) {
			chunk->cells[yCoord][xCoord].alive = 1;
		} else {
			chunk->cells[yCoord][xCoord].alive = 0;
		}
		if (chunk->cells[yCoord][xCoord].alive) {
			for (x = -1; x <= 1; x++) {
				for (y = -1; y <= 1; ++y) {
					elementToSearch[0] = xCoord + x;				
					elementToSearch[1] = yCoord + y;				
					if (!found(elementToSearch, tempArray, tempArrayIndex)) {
						tempArray[tempArrayIndex][0] = xCoord + x;			
						tempArray[tempArrayIndex][1] = yCoord + y;			
						++tempArrayIndex;
					}
				}
			}
		}
	}
	printInfo(*chunk);
	copyArray(chunk, tempArray);
	chunk->cellsToTestCount = tempArrayIndex;
}

void copyArray (Chunk * chunk, int arr[][2]) {
	memcpy(&chunk->cellsToTest, arr, sizeof(chunk->cellsToTest));
}
