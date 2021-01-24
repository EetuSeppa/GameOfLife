#ifndef ARR_FUNC_
#define ARR_FUNC_

#include "helpers.h"

void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount);

void copyArray (Chunk * chunk, int arr2[][2]);
void printInfo (Chunk chunk);
void initialTestedCells (Chunk * chunk, DrawnChunk *drawnVersion);
void testAliveNeighbors (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int *renderedChunkCount);
int cellAliveState (Chunk * chunk, DrawnChunk * drawnVersion);
void copyArray (Chunk * chunk, int arr[][2]);
void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount);
void addAliveStatesToNeighbors (int neighborChunkCoords[3][2], int cellCoordsInNeighbors[3][3][2], int neighborChunkCount, Chunk * chunk, int cellXCoord, int cellYCoord, Chunk * firstChunk, Chunk ** lastChunk, int *rendredChunkCount);
void initGlobalVariable(void);
extern NeighChunkStruct * collisionNeighbor;

#endif
