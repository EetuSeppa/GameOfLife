#include "drawGrid.h"
#include "readRLE.h"
#include <stdio.h>

//If main has arguments, call RLE reading function with filename passed in arguments.
//Reading function returns a pointer to linked list with chunks rendered by reading the file
//Otherwise drawgrid function gets called with NULL
int main (int argc, char * argv[]) {
	Chunk * chunkList, * lastChunk;

	if (argc == 1) {
		drawGrid(NULL, NULL);
	} else if (argc == 2) {
		chunkList = readRLEFile(argv[1], &lastChunk);	
		printf("REAding\n");
		drawGrid(chunkList, lastChunk);
	} else {
		printf("Expected one argument!\n\n");
		return 1;
	}

}
