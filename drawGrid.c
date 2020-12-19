#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
void drawGrid (void) {		
	int lineDistance, i, drawnPosX, drawnPosY, worldPosX, worldPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	lineDistance = 10;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	mouseOffsetX = mouseOffsetY = 0;
	worldPosX = worldPosY = 0;
	prevMouseOffsetX = prevMouseOffsetY = 0;
	while (!WindowShouldClose()) {
		
		//Get mouse movements while pressed
		//TODO: Implement position in wolrd 
		//TODO: Implement zooming
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			initMouseX = GetMouseX() - worldPosX;
			initMouseY = GetMouseY() - worldPosY;
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			prevMouseOffsetX = mouseOffsetX;
			prevMouseOffsetY = mouseOffsetY;
			mouseOffsetX = GetMouseX() - initMouseX;
			mouseOffsetY = GetMouseY() - initMouseY;
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			//prevMouseOffsetX = 0;
			//prevMouseOffsetY = 0;
		}
		
		BeginDrawing();
			ClearBackground(RAYWHITE);
			drawnPosY = drawnPosX = 0;
			worldPosX += mouseOffsetX - prevMouseOffsetX;
			worldPosY += mouseOffsetY - prevMouseOffsetY;
			for (drawnPosX = 0;  drawnPosX < SCREEN_WIDTH - 1; drawnPosX += lineDistance) {
				if (drawnPosX <= SCREEN_WIDTH) {
					DrawLine(worldPosX + drawnPosX, worldPosY, worldPosX + drawnPosX, SCREEN_HEIGHT + worldPosY, GRAY);
				}
				if (drawnPosY <= SCREEN_HEIGHT) {
					DrawLine(worldPosX, worldPosY + drawnPosY, SCREEN_WIDTH  + worldPosX, worldPosY + drawnPosY, GRAY);
				}
	
				drawnPosY += lineDistance;
			}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
