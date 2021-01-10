#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"
#include "raylib.h"

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
//Return null pointer if not found
Chunk * findIndex (int elementToSearch[], Chunk * firstChunk) {
	Chunk * curChunk;
	
	curChunk = firstChunk;
	do {
		if 	(curChunk->coord[0] == elementToSearch[0]) {
			if (curChunk->coord[1] == elementToSearch[1]) {
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
int * copySelectionArea (int x1, int y1, int x2, int y2, Chunk * firstChunk, int * sizeX, int * sizeY, int worldPosX, int worldPosY, int lineDistance) {
	//Assume that selection end indices are always bigger than starts for now
	int curChunkIndexX, curChunkIndexY, startPosX, startPosY, endPosX, endPosY, i, j;
	int searchArr[2];
	
	Chunk * selectionChunk;
	curChunkIndexX = indexOfChunkFromCoord (x1, worldPosX, lineDistance);
	curChunkIndexY = indexOfChunkFromCoord (y1, worldPosX, lineDistance);

	searchArr[0] = curChunkIndexX;
	searchArr[1] = curChunkIndexY;

	selectionChunk = findIndex(searchArr, firstChunk);

	startPosX = arrayIndexFromCoord (x1, worldPosX, lineDistance);
	startPosY = arrayIndexFromCoord (y1, worldPosY, lineDistance);

	endPosX = arrayIndexFromCoord (x2, worldPosX, lineDistance);
	endPosY = arrayIndexFromCoord (y2, worldPosY, lineDistance);

	*sizeX = endPosX - startPosX;	
	*sizeY = endPosY - startPosY;	

	int (*selectionArr)[*sizeX] = malloc(sizeof(int[*sizeY][*sizeX]));

	for (i = 0; i < *sizeY; ++i) {
		for (j = 0; j < *sizeX; ++j) {
			selectionArr[i][j] = selectionChunk->cells[startPosY + i][startPosX + j].alive;
		}
	}


	return *selectionArr;	
}

void pasteSelectionArea (int * copiedArea, int x1, int y1, Chunk * firstChunk, int sizeX, int sizeY, int worldPosX, int worldPosY, int lineDistance) {
	int i, j, curChunkIndexX, curChunkIndexY, startIndexX, startIndexY;
	Chunk * destinationChunk;
	int searchArr[2];

	//Have to do this for pointer arithmetic to work properly
	int (*tempSelection)[sizeX] = malloc(sizeof(int[sizeY][sizeX]));
	memcpy(tempSelection, copiedArea, sizeof(int[sizeY][sizeX]));

	curChunkIndexX = indexOfChunkFromCoord (x1, worldPosX, lineDistance);
	curChunkIndexY = indexOfChunkFromCoord (y1, worldPosY, lineDistance);

	searchArr[0] = curChunkIndexX;	
	searchArr[1] = curChunkIndexY;	
	destinationChunk = findIndex(searchArr, firstChunk);

	startIndexX = arrayIndexFromCoord (x1, worldPosX, lineDistance);
	startIndexY = arrayIndexFromCoord (y1, worldPosY, lineDistance);

	for (i = 0; i < sizeY; ++i) {
		for (j = 0; j < sizeX; ++j) {
			destinationChunk->cells[startIndexY + i][startIndexX + j].alive = tempSelection[i][j];
			searchArr[0] = startIndexX + j;
			searchArr[1] = startIndexY + j;
			destinationChunk->cellsToTest[destinationChunk->cellsToTestCount][0] = startIndexX + j; 
			destinationChunk->cellsToTest[destinationChunk->cellsToTestCount][1] = startIndexY + i; 
			destinationChunk->cellsToTestCount += 1; 
		}
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
