#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define INITIAL_GRID_WIDTH 15
void drawGrid (void) {		

	//Load font
	Font GetFontDefault();
	int lineDistance, i, drawnPosX, drawnPosY, worldPosX, worldPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	int screenWidth, screenHeight;
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;
	double zoom;
	lineDistance = INITIAL_GRID_WIDTH;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	mouseOffsetX = mouseOffsetY = 0;
	worldPosX = worldPosY = 0;
	prevMouseOffsetX = prevMouseOffsetY = 0;
	zoom = 1.000;
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
		
		zoom = GetMouseWheelMove();
		lineDistance -= (int)zoom;
		/*
		screenWidth *= lineDistance / INITIAL_GRID_WIDTH;
		screenHeight *= lineDistance / INITIAL_GRID_WIDTH;
		*/

		BeginDrawing();
			//Rectangle for testing
			DrawRectangle(worldPosX, worldPosY, lineDistance, lineDistance, GRAY);

			ClearBackground(RAYWHITE);
			drawnPosY = drawnPosX = 0;
			worldPosX += mouseOffsetX - prevMouseOffsetX;
			worldPosY += mouseOffsetY - prevMouseOffsetY;

			//TODO: Declare screen width as a variable that is edited by zoom
			//TODO: Create effect of infinite world
			while (drawnPosX < worldPosX + screenWidth) {
				if (drawnPosX <= screenWidth) {
					DrawLine(worldPosX + drawnPosX, worldPosY, worldPosX + drawnPosX, SCREEN_HEIGHT + worldPosY, GRAY);
				}
				if (drawnPosY <= screenHeight) {
					DrawLine(worldPosX, worldPosY + drawnPosY, SCREEN_WIDTH  + worldPosX, worldPosY + drawnPosY, GRAY);
				}
	
				drawnPosX += lineDistance;
				drawnPosY += lineDistance;
				
			}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
