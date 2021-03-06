#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"
#include "raylib.h"
#include <math.h>

//Horribly inefficient. TODO: Binary search for x coordinate and insert in order
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

//Return null pointer if not found in linked list of chunks in memory
Chunk * findIndex (int elementToSearch[], Chunk * firstChunk) {
	Chunk * curChunk;
	
	curChunk = firstChunk;
	do {
		if 	(curChunk->x == elementToSearch[0]) {
			if (curChunk->y == elementToSearch[1]) {
				return curChunk;
			}
		}
		if (curChunk->nextChunk != NULL)
			curChunk = curChunk->nextChunk;
		else 
			break;
	} while (1);

	return NULL;
}

//Allocates memory for new chunk, increments chunkIndex
Chunk * renderChunk(Chunk ** lastChunk, int xCoord, int yCoord) {
	Chunk * curChunk;

	curChunk = malloc(sizeof(Chunk));
	(*lastChunk)->nextChunk = curChunk;
	curChunk->x = xCoord;
	curChunk->y = yCoord;
	curChunk->cellsToTestCount = 0;
	initializeZeroArray(curChunk->cells);
	
	curChunk->nextChunk = NULL;
	
	*lastChunk = curChunk;
	return curChunk;
}

int * copySelectionArea (int x1, int y1, int x2, int y2, Chunk * firstChunk, int * sizeX, int * sizeY, int worldPosX, int worldPosY, int lineDistance) {
	//Assume that selection end indices are always bigger than starts for now
	int curChunkIndexX, curChunkIndexY, startPosX, startPosY, endPosX, endPosY, i, j, currentArrPosX, currentArrPosY;
	int searchArr[2];
	
	Chunk * initialSelectionChunk;
	Chunk * currentSelectionChunk;
	curChunkIndexX = indexOfChunkFromCoord (x1, worldPosX, lineDistance);
	curChunkIndexY = indexOfChunkFromCoord (y1, worldPosY, lineDistance);

	searchArr[0] = curChunkIndexX;
	searchArr[1] = curChunkIndexY;

	initialSelectionChunk = findIndex(searchArr, firstChunk);

	startPosX = arrayIndexFromCoord (x1, worldPosX, lineDistance);
	startPosY = arrayIndexFromCoord (y1, worldPosY, lineDistance);

	endPosX = arrayIndexFromCoord (x2, worldPosX, lineDistance);
	endPosY = arrayIndexFromCoord (y2, worldPosY, lineDistance);

	*sizeX = (abs(((x1 - worldPosX % lineDistance) / lineDistance) - ((x2 - worldPosX % lineDistance) / lineDistance))) + 1;
	*sizeY = (abs(((y1 - worldPosY % lineDistance) / lineDistance) - ((y2 - worldPosY % lineDistance) / lineDistance))) + 1;

	int (*selectionArr)[*sizeX] = malloc(sizeof(int[*sizeY][*sizeX]));

	//Logic for copying from arrays
	currentArrPosX = startPosX;
	currentArrPosY = startPosY;
	currentSelectionChunk = initialSelectionChunk;
	for (i = 0; i < *sizeY; ++i) {
		currentArrPosX = startPosX;
		if (currentArrPosY > ARR_SIZE - 1) {
			currentArrPosY = 0;
			searchArr[1]++;
			currentSelectionChunk = findIndex(searchArr, firstChunk);
		}
		for (j = 0; j < *sizeX; ++j) {
			if (currentArrPosX > ARR_SIZE - 1) {
				searchArr[0]++;
				currentSelectionChunk = findIndex(searchArr, firstChunk);
				searchArr[0]--;	
				currentArrPosX = 0;
			}
			if (currentSelectionChunk != NULL) {
				selectionArr[i][j] = currentSelectionChunk->cells[currentArrPosY][currentArrPosX].alive;
			} else {
				selectionArr[i][j] = 0;
			}
			++currentArrPosX;
		}
		++currentArrPosY;
		currentSelectionChunk = findIndex(searchArr, firstChunk);
	}


	return *selectionArr;	
}

void pasteSelectionArea (int * copiedArea, int x1, int y1, Chunk * firstChunk, Chunk ** lastChunk, int sizeX, int sizeY, int worldPosX, int worldPosY, int lineDistance, int * renderedChunkCount) {
	int i, j, curChunkIndexX, curChunkIndexY, startIndexX, startIndexY, curPosX, curPosY;
	Chunk * destinationChunk;
	Chunk * curDestinationChunk;
	int searchArr[2];

	//Have to do this for pointer arithmetic to work properly
	int (*tempSelection)[sizeX] = malloc(sizeof(int[sizeY][sizeX]));
	memcpy(tempSelection, copiedArea, sizeof(int[sizeY][sizeX]));

	curChunkIndexX = indexOfChunkFromCoord (x1, worldPosX, lineDistance);
	curChunkIndexY = indexOfChunkFromCoord (y1, worldPosY, lineDistance);

	searchArr[0] = curChunkIndexX;	
	searchArr[1] = curChunkIndexY;	
	destinationChunk = findIndex(searchArr, firstChunk);
	if (destinationChunk == NULL) {
		destinationChunk = renderChunk(lastChunk, curChunkIndexX, curChunkIndexY);
		*renderedChunkCount += 1;

	}

	startIndexX = arrayIndexFromCoord (x1, worldPosX, lineDistance);
	startIndexY = arrayIndexFromCoord (y1, worldPosY, lineDistance);

	curPosY = startIndexY;
	curDestinationChunk = destinationChunk;
	for (i = 0; i < sizeY; ++i) {
		curPosX = startIndexX;
		if (curPosY > ARR_SIZE - 1) {
			++searchArr[1];
			curDestinationChunk = findIndex(searchArr, firstChunk);
			if (curDestinationChunk == NULL) {
				curDestinationChunk = renderChunk(lastChunk, searchArr[0], searchArr[1]);
				*renderedChunkCount += 1;
			}
			curPosY = 0;
		}
		for (j = 0; j < sizeX; ++j) {
			if (curPosX > ARR_SIZE - 1) {
				++searchArr[0];
				curDestinationChunk = findIndex(searchArr, firstChunk);
				if (curDestinationChunk == NULL) {
					curDestinationChunk = renderChunk(lastChunk, searchArr[0], searchArr[1]);
					*renderedChunkCount += 1;
				}
				curPosX = 0;
				--searchArr[0];
			}
			curDestinationChunk->cells[curPosY][curPosX].alive = tempSelection[i][j];
			curDestinationChunk->cellsToTest[curDestinationChunk->cellsToTestCount][0] = curPosX; 
			curDestinationChunk->cellsToTest[curDestinationChunk->cellsToTestCount][1] = curPosY; 
			curDestinationChunk->cellsToTestCount += 1; 
			curPosX++;
		}
		curPosY++;
		curDestinationChunk = findIndex(searchArr, firstChunk);
	}
	free(tempSelection);
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

int indexOfChunkFromCoord (int clickCoord, int worldPos, int lineDistance) {
	int indexOfChunk;

	indexOfChunk = ((clickCoord - worldPos) / lineDistance) / ARR_SIZE;
	
	if (clickCoord - worldPos < 0)
		--indexOfChunk;

	return indexOfChunk;
}

int arrayIndexFromCoord (int clickCoord, int worldPos, int lineDistance) {
	int indexOfCell;

	indexOfCell = ((clickCoord - worldPos) / lineDistance) % ARR_SIZE;


	if (clickCoord - worldPos < 0)
		indexOfCell += 49;	
	
	return indexOfCell;
}
void drawSelectionRect (int x1, int y1, int x2, int y2) {
	float thickness = 5.5;
	Vector2 pos1, pos2;
	
	pos1.x = x1; pos1.y = y1;
	pos2.x = x2; pos2.y = y1;
	DrawLineEx(pos1, pos2, thickness, RED);

	pos1.x = x1; pos1.y = y2;
	pos2.x = x2; pos2.y = y2;
	DrawLineEx(pos1, pos2, thickness, RED);

	pos1.x = x1; pos1.y = y1;
	pos2.x = x1; pos2.y = y2;
	DrawLineEx(pos1, pos2, thickness, RED);
	
	pos1.x = x2; pos1.y = y1;
	pos2.x = x2; pos2.y = y2;
	DrawLineEx(pos1, pos2, thickness, RED);

}

