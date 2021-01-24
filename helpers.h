#ifndef HELP_H_
#define HELP_H_
#define ARR_SIZE 50

#include <pthread.h>

pthread_t aliveStateUpdateThread;
pthread_mutex_t chunkUpdateMutex;
pthread_cond_t chunkUpdateCond;
int drawingCompleteBool;

pthread_mutex_t threadStartTimingMutex;
pthread_cond_t threadStartTimingCond;
int initialMutexLockBool;

typedef struct coord {
	int x, y;
} Coord;

typedef struct drawnChunk {
	struct drawnChunk * nextChunk;
	Coord cellCoords[ARR_SIZE * ARR_SIZE];
	int cellCount;
	int x, y; //Position of chunk in world

} DrawnChunk;

typedef struct neighChunkCoords {
	int neighChunkCoords[3][2];
	int neighCellsInNeighChunks[3][3][2];
	int neighChunkCount;

} NeighChunkCoords;

typedef struct neighChunkStruct {
	NeighChunkCoords upLeft, upRight, downLeft, downRight, left, up, right, down;


} NeighChunkStruct;

typedef struct cell {
	int alive;	//1 for alive, 0 for dead
	int aliveNeighbors;


} Cell;

typedef struct chunk {
	struct chunk * nextChunk; //Points to next rendered chunk
	Cell cells[ARR_SIZE][ARR_SIZE]; //Array of cells
	int cellsToTest[ARR_SIZE * ARR_SIZE][2]; //Cell coordinates that need to be tested
	int cellsToTestCount; //Count of tested cells
	int x, y; //Coordinates of chunk in world
} Chunk;

void initializeZeroArray (Cell arr[][ARR_SIZE]);
Chunk * renderChunk(Chunk ** lastChunk, int xCoord, int yCoord);
int indexOfChunkFromCoord (int clickCoord, int worldPos, int lineDistance);
int arrayIndexFromCoord (int clickCoord, int worldPos, int lineDistance);
void drawSelectionRect (int x1, int y1, int x2, int y2);
int * copySelectionArea (int x1, int y1, int x2, int y2, Chunk * firstChunk, int * sizeX, int * sizeY, int worldPosX, int worldPosY, int lineDistance);
Chunk * findIndex (int elementToSearch[], Chunk * firstChunk);
void pasteSelectionArea (int * copiedArea, int x1, int x2, Chunk * firstChunk, Chunk ** lastChunk, int sizeX, int sizeY, int worldPosX, int worldPosY, int lineDistance, int * renderedChunkCount);
int found (int elementToSearch[], int arr[][2], int limit);
void initializeNeighborCoords (NeighChunkStruct ** structToInit);

#endif
