#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
void drawGrid (void) {		
	int lineDistance, i, drawnPosX, drawnPosY, worldPosX, worldPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY;
	lineDistance = 10;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	mouseOffsetX = mouseOffsetY = 0;
	while (!WindowShouldClose()) {
		
		//Get mouse movements while pressed
		//TODO: Implement panning
		//TODO: Implement zooming
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			initMouseX = GetMouseX();
			initMouseY = GetMouseY();
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			mouseOffsetX = GetMouseX() - initMouseX;
			mouseOffsetY = GetMouseY() - initMouseY;
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			mouseOffsetX = 0;
			mouseOffsetY = 0;
		}
		
		BeginDrawing();
			ClearBackground(RAYWHITE);
			drawnPosY = drawnPosX = 0;
			for (drawnPosX = 0;  drawnPosX < SCREEN_WIDTH - 1; drawnPosX += lineDistance) {
				worldPosX += mouseOffsetX;
				worldPosY += mouseOffsetY;
				if (drawnPosX + mouseOffsetX <= SCREEN_WIDTH) {
					DrawLine(drawnPosX + mouseOffsetX, mouseOffsetY, drawnPosX + mouseOffsetX, SCREEN_HEIGHT + mouseOffsetY, GRAY);
				}
				if (drawnPosY <= SCREEN_HEIGHT) {
					DrawLine(mouseOffsetX, drawnPosY + mouseOffsetY, SCREEN_WIDTH  + mouseOffsetX, drawnPosY + mouseOffsetY, GRAY);
					;
				}
	
				drawnPosY += lineDistance;
			}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
