GameOfLife: main.c helpers.c arrays.c drawGrid.c readRLE.c aliveStateUpdate.c
	gcc -I/usr/local/Cellar/raylib/3.0.0/include -pthread -lraylib -o gameOfLife drawGrid.c aliveStateUpdate.c helpers.c main.c arrays.c readRLE.c
