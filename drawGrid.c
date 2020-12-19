#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
int main (void) {		
	int lineDistance, i, linePos;
	lineDistance = 10;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	while (!WindowShouldClose()) {
	
	BeginDrawing();
		ClearBackground(RAYWHITE);
		for (linePos = 0;  linePos < SCREEN_WIDTH - 1; linePos += lineDistance) {
			if (linePos <= SCREEN_WIDTH)
				DrawLine(linePos, 0, linePos, SCREEN_HEIGHT, GRAY);
			if (linePos <= SCREEN_HEIGHT)
				DrawLine(0, linePos, SCREEN_WIDTH, linePos, GRAY);
		}
	EndDrawing();
	}
}
