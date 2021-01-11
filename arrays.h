#ifndef ARR_FUNC_
#define ARR_FUNC_

#include "helpers.h"

void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount);

void copyArray (Chunk * chunk, int arr2[][2]);
void printInfo (Chunk chunk);
void initialTestedCells (Chunk * chunk);
void testAliveNeighbors (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int *renderedChunkCount);
int cellAliveState (Chunk * chunk);
void copyArray (Chunk * chunk, int arr[][2]);
void chunkCollision (Chunk * chunk, Chunk ** lastChunk, Chunk * firstChunk, int cellXCoord, int cellYCoord, int *renderedChunkCount);

#endif
