#include "readRLE.h"
#include <stdlib.h>
#include "helpers.h"
#include <stdio.h>
#include <string.h>

int strToNum (char str[]) {
	int len, num, i, multiplier;	
	len = strlen(str);
	multiplier = 1;
	num = 0;
	for (i = 0; i < len - 1; ++i) {
		multiplier *= 10;

	}
	for (i = 0; i < len; ++i) {
		num += (str[i] - '0') * multiplier;
		multiplier /= 10;

	}
	return num;

}
Chunk * readRLEFile (char fileName[], Chunk ** lastChunkOfList) {
	int i, j, tokenPos, numToken, cellPosX, cellPosY, chunkPosX, chunkPosY;
	int searchArr[2];
	Chunk * firstChunk, * lastChunk, * curChunk;
	
	//Start position is zero
	cellPosX = cellPosY = chunkPosX = chunkPosY = 0;

	firstChunk = malloc(sizeof(Chunk));
	firstChunk->coord[0] = chunkPosX;
	firstChunk->coord[1] = chunkPosY;
	firstChunk->cellsToTestCount = 0;
	initializeZeroArray(firstChunk->cells);
	firstChunk->nextChunk = NULL;

	lastChunk = firstChunk;
	curChunk = firstChunk;

	
	char line[100];
	char numTokenStr[20];
	char typeToken;
	FILE *fp = fopen(fileName, "r");	

	fgets(line, 100, fp);
	putchar('\n');
	while (feof(fp) == 0) {
		i = 0;
		while (typeToken != '!') {
			tokenPos = 0;
			while (line[i] >= '0' && line[i] <= '9') {
				numTokenStr[tokenPos] = line[i];
				++tokenPos;
				++i;
			}
			numTokenStr[tokenPos] = '\0';
			numToken = strToNum(numTokenStr);
			typeToken = line[i];
			++i;
			if (typeToken == '\n')
				break;

			for (; numToken >= 0; --numToken) {
				if (cellPosX > ARR_SIZE - 1 || cellPosY > ARR_SIZE - 1) {
					if (cellPosX > ARR_SIZE - 1) {
						cellPosX = 0;
						chunkPosX++;
					} else {
						cellPosY = 0;
						chunkPosY++;
					}

					searchArr[0] = chunkPosX;
					searchArr[1] = chunkPosY;
					if ((curChunk = findIndex(searchArr, firstChunk)) == NULL) {
						curChunk = renderChunk(&lastChunk, chunkPosX, chunkPosY);
					}
				}
				if (typeToken == '$') {
					cellPosY++;
					cellPosX = -1; //Set to -1 for increment to work properly
					chunkPosX = 0;
					searchArr[0] = chunkPosX;
					searchArr[1] = chunkPosY;
					if ((curChunk = findIndex(searchArr, firstChunk)) == NULL) {
						curChunk = renderChunk(&lastChunk, chunkPosX, chunkPosY);
					}
				} else if (typeToken == 'o') {
					curChunk->cells[cellPosY][cellPosX].alive = 1;
					curChunk->cellsToTest[curChunk->cellsToTestCount][0] = cellPosX;
					curChunk->cellsToTest[curChunk->cellsToTestCount][1] = cellPosY;
					curChunk->cellsToTestCount += 1;
				}

				++cellPosX;
				if (numToken == 1)
					break;
			}
				
		}
		fgets(line, 100, fp);
	}

	fclose(fp);
	*lastChunkOfList = lastChunk;
	return firstChunk;
}

