#include <stdlib.h>
#include <string.h>

void initializeZeroArray (Cell arr[][ARR_SIZE]) {
	int i, j;
	
	for (i = 0; i < 50; ++i) {
		for (j = 0; j < 50; ++j) {
			arr[i][j].alive = 0;
		}

	}
}
