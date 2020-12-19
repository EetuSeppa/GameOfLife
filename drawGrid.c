#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
void drawGrid (void) {		
	int lineDistance, i, screenPosX, screenPosY, worldPosX, worldPosY, drawnPosX, drawnPosY;
	int initMouseX, initMouseY;
	lineDistance = 10;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	while (!WindowShouldClose()) {
		
		//Get mouse movements while pressed
		//TODO: Implement panning
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			initMouseX = GetMouseX();
			initMouseY = GetMouseY();
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			

		}
		/*if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			initMouseX = 0;
			initMouseY = 0;
		}
		*/
		BeginDrawing();
			ClearBackground(RAYWHITE);
			drawnPosY = drawnPosX = 0;
			for (drawnPosX = 0;  drawnPosX < SCREEN_WIDTH - 1; drawnPosX += lineDistance) {
				if (drawnPosX <= SCREEN_WIDTH)
					DrawLine(drawnPosX, 0, drawnPosX, SCREEN_HEIGHT, GRAY);
				if (drawnPosY <= SCREEN_HEIGHT)
					DrawLine(0, drawnPosY, SCREEN_WIDTH, drawnPosY, GRAY);
	
				drawnPosY += lineDistance;
			}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
