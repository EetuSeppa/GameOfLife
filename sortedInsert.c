#include "sortedInsert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

void insertIfNotIncluded (Chunk * chunk, int x, int y) {
	int i, middle, lowLimit, highLimit;
	highLimit = chunk->cellsToTestCount; 
	lowLimit = 0;

	if (highLimit == 0) {
		printf("got here once\n");
		chunk->cellsToTest[0][0] = x;
		chunk->cellsToTest[0][1] = y;
		chunk->cellsToTestCount += 1;

	}

	while (1) {
		middle = lowLimit + ((highLimit - lowLimit) / 2);	
		printf("%d\n", middle);
		if ((highLimit - lowLimit <= 1 && chunk->cellsToTest[middle][0] != x)) {
			if (chunk->cellsToTest[middle][0] > x) {
				printf("got1\n");
				memmove(chunk->cellsToTest[middle + 1], 
						chunk->cellsToTest[middle], 
						sizeof(int[2]) * ((ARR_SIZE * ARR_SIZE) - middle + 1)
				);

				chunk->cellsToTest[middle][0] = x;	
				chunk->cellsToTest[middle][1] = y;	
				chunk->cellsToTestCount += 1;

				break;

			} else if (chunk->cellsToTest[middle][0] < x) {
				printf("got2\n");
				memmove(chunk->cellsToTest[middle + 2],
						chunk->cellsToTest[middle + 1],
						sizeof(int[2]) * ((ARR_SIZE * ARR_SIZE) - middle + 2)
				);

				chunk->cellsToTest[middle + 1][0] = x;	
				chunk->cellsToTest[middle + 1][1] = y;	
				chunk->cellsToTestCount += 1;

				break;

			} else if (middle == 0) {
				printf("got3\n");
				chunk->cellsToTest[middle][0] = x;	
				chunk->cellsToTest[middle][1] = y;	
				chunk->cellsToTestCount += 1;

				break;

			}

		} else if (chunk->cellsToTest[middle][0] == x) {
			testYCoord(chunk, x, y, middle);	
			break;
		} else if (chunk->cellsToTest[middle][0] > x) {
			highLimit = middle;	
		
		} else if (chunk->cellsToTest[middle][0] < x) {
			lowLimit = middle;
		}
	}

}

void testYCoord (Chunk * chunk, int x, int y, int index) {
	if (chunk->cellsToTest[index][1] > y && chunk->cellsToTest[index][0] == x) {
		while (chunk->cellsToTest[index][1] > y && chunk->cellsToTest[index][0] == x) {
			--index;
		}
		if (chunk->cellsToTest[index][1] != y) {
			memmove(chunk->cellsToTest[index + 2], 
					chunk->cellsToTest[index + 1],
					sizeof(int[2]) * ((ARR_SIZE * ARR_SIZE) - index + 2)
			);
			chunk->cellsToTest[index + 1][0] = x;
			chunk->cellsToTest[index + 1][1] = y;
			chunk->cellsToTestCount += 1;

		}
	} else if (chunk->cellsToTest[index][1] < y && chunk->cellsToTest[index][0] == x) {
		while (chunk->cellsToTest[index][1] > y && chunk->cellsToTest[index][0] == x) {
			++index;
		}

		if (chunk->cellsToTest[index][1] != y) {
			memmove(chunk->cellsToTest[index + 1], 
					chunk->cellsToTest[index],
					sizeof(int[2]) * ((ARR_SIZE * ARR_SIZE) - index + 1)
			);
			chunk->cellsToTest[index + 1][0] = x;
			chunk->cellsToTest[index + 1][1] = y;
			chunk->cellsToTestCount += 1;

		}

	}
}
