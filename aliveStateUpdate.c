#include "aliveStateUpdate.h"
#include "helpers.h"
#include <pthread.h>
#include "arrays.h"
#include <stdlib.h>
#include <stdio.h>


//TODO: Crashes when moving to different chunk

void *aliveStateThread (void *args) {
	
	ThreadArgs *threadData = (ThreadArgs*)args;
	DrawnChunk *currentDrawnChunk, *tempDrawnChunk;
	Chunk *curChunk, *previousChunk;
	//Free previous drawing list here if != NULLl
	pthread_mutex_lock(&chunkUpdateMutex);

	pthread_mutex_lock(&threadStartTimingMutex);
	initialMutexLockBool = 1;
	pthread_mutex_unlock(&threadStartTimingMutex);
	pthread_cond_signal(&threadStartTimingCond);

	while (1) {
		drawingCompleteBool = 0;
		if (*(threadData->previousFirstDrawnChunk) != NULL) { //Free previous linked list
			currentDrawnChunk = *(threadData->previousFirstDrawnChunk);
			while(1) {
				if (currentDrawnChunk->nextChunk != NULL) {
					tempDrawnChunk = currentDrawnChunk;	
					currentDrawnChunk = currentDrawnChunk->nextChunk;
					free(tempDrawnChunk);
				} else {
					//Causes error sometimes, race condition (freed pointer error)?
					free(currentDrawnChunk);
					currentDrawnChunk = NULL;
					break;
				}
			}
		}

		curChunk = threadData->firstChunk;
		while (1) {
			testAliveNeighbors(curChunk, threadData->lastChunk, threadData->firstChunk, threadData->renderedChunkCount);
			if (curChunk->nextChunk != NULL) {
				curChunk = curChunk->nextChunk;
			} else {
				break;
			}
		} 
		//Not enough drawnchunks are generated
		curChunk = threadData->firstChunk;
		previousChunk = NULL;
		*(threadData->nextFirstDrawnChunk) = malloc(sizeof(DrawnChunk));
		currentDrawnChunk = *(threadData->nextFirstDrawnChunk);
		while (1) {
			currentDrawnChunk->cellCount = 0; 									//Initialize cell count to 0
			if ((cellAliveState(curChunk, currentDrawnChunk)) == 0) { 			//If cell has zero alive cells free it from list
				if (previousChunk != NULL && curChunk->nextChunk != NULL) { //Keep middle chunk in memory
					previousChunk->nextChunk = curChunk->nextChunk;
					free(curChunk);
					free(currentDrawnChunk);
					tempDrawnChunk->nextChunk = malloc(sizeof(DrawnChunk));
					currentDrawnChunk = tempDrawnChunk->nextChunk;
					curChunk = previousChunk->nextChunk;
					continue;
				}
			}
			if (curChunk->nextChunk != NULL) {
				previousChunk = curChunk;
				curChunk = curChunk->nextChunk;
				tempDrawnChunk = currentDrawnChunk;

				currentDrawnChunk->nextChunk = malloc(sizeof(DrawnChunk));
				currentDrawnChunk = currentDrawnChunk->nextChunk;
			} else { 
				currentDrawnChunk->nextChunk = NULL;
				break; 
			}
		} 
		while (!drawingCompleteBool) {
			pthread_cond_wait(&chunkUpdateCond, &chunkUpdateMutex);
		}
	}

	//create new list for drawing here and pass the pointer to a variable which drawing can access when it has mutex locked?
		//Should do this in cell alivestate function
	
}
