#ifndef HELP_H_
#define HELP_H_
#define ARR_SIZE 50

typedef struct neighborChunks {
	


} NeighborChunks;

typedef struct cell {
	//1 for alive, 0 for dead
	int alive;
	int aliveNeighbors;


} Cell;

typedef struct chunk {
	struct chunk * nextChunk; //Points to next rendered chunk
	Cell cells[ARR_SIZE][ARR_SIZE]; //Array of cells
	int cellsToTest[ARR_SIZE * ARR_SIZE][2]; //Cell coordinates that need to be tested
	int cellsToTestCount; //Count of tested cells
	int coord[2]; //Coordinate of chunk in world
} Chunk;

void initializeZeroArray (Cell arr[][ARR_SIZE]);
Chunk * renderChunk(Chunk * lastChunk, int xCoord, int yCoord);
int indexOfChunkFromCoord (int clickCoord, int worldPos, int lineDistance);
int arrayIndexFromCoord (int clickCoord, int worldPos, int lineDistance);
void drawSelectionRect (int x1, int y1, int x2, int y2);
int * copySelectionArea (int x1, int y1, int x2, int y2, Chunk * firstChunk, int * sizeX, int * sizeY, int worldPosX, int worldPosY, int lineDistance);
Chunk * findIndex (int elementToSearch[], Chunk * firstChunk);
void pasteSelectionArea (int * copiedArea, int x1, int x2, Chunk * firstChunk, int sizeX, int sizeY, int worldPosX, int worldPosY, int lineDistance);
int found (int elementToSearch[], int arr[][2], int limit);

#endif
