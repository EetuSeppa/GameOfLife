GameOfLife: main.c helpers.c arrays.c drawGrid.c
	gcc -I/usr/local/Cellar/raylib/3.0.0/include -lraylib -o gameOfLife drawGrid.c helpers.c main.c arrays.c
