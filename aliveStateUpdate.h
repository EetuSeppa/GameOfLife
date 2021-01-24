#ifndef ALV_THREAD_
#define ALV_THREAD_

#include "helpers.h"

typedef struct threadArgs {
	Chunk *firstChunk, **lastChunk;
	int * renderedChunkCount;
	DrawnChunk **previousFirstDrawnChunk, **nextFirstDrawnChunk;
} ThreadArgs;

void *aliveStateThread (void *args);



#endif
