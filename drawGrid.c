#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define INITIAL_GRID_WIDTH 15
void drawGrid (void) {		

	//Load font
	Font GetFontDefault();
	int lineDistance, i, drawnPosX, drawnPosY, worldPosX, worldPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	double zoom;
	lineDistance = INITIAL_GRID_WIDTH;

	//Lisää paneroitujen koordinaattien muuttaminen näytölle
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	mouseOffsetX = mouseOffsetY = 0;
	worldPosX = worldPosY = 0;
	prevMouseOffsetX = prevMouseOffsetY = 0;
	zoom = 1.000;
	while (!WindowShouldClose()) {
		
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

		}
		
		zoom = GetMouseWheelMove();
		lineDistance -= (int)zoom;

		BeginDrawing();
			//Rectangles for testing
			DrawRectangle(worldPosX, worldPosY, lineDistance, lineDistance, GRAY);
			DrawRectangle(worldPosX + lineDistance * 2, worldPosY + lineDistance * 2, lineDistance, lineDistance, GRAY);
			
			ClearBackground(RAYWHITE);
			worldPosX += mouseOffsetX - prevMouseOffsetX;
			worldPosY += mouseOffsetY - prevMouseOffsetY;


			drawnPosY = worldPosY % lineDistance;
			drawnPosX = worldPosX % lineDistance; 
			
			int drawnLinesX = 0;
			int drawnLinesY = 0;
			while (drawnPosX < SCREEN_WIDTH) {
				if (drawnPosX <= SCREEN_WIDTH) {
					DrawLine(drawnPosX, 0, drawnPosX, SCREEN_HEIGHT, GRAY);
					++drawnLinesX;
				}
				if (drawnPosY <= SCREEN_HEIGHT) {
					DrawLine(0, drawnPosY, SCREEN_WIDTH, drawnPosY, GRAY);
					++drawnLinesY;
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
