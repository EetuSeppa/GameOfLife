/*
	TODO:

	-If alive cell "collides" with corner or edge of array, start checking in next array.
		-If collision in edges happens, check if array is alive yet, if not malloc space for array and insert edge cells to cellsToCheck of new array. If array is already rendered, just do the latter.  	
			-Needs logic for finding correct index of colliding array

	SHOULD DO THIS FIRST
	-Implement placing of cells into other arrays besides center array.
		-Needs functionality for allocating memory for new arrays and giving arrays correct coordinates
		-Needed also: testing alive conditions of multiple arrays

	-v Implement smart checking of cells inside each array.
		-Needs faster algorithm for searching. (Is searching really needed?)

*/
#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include "arrays.c"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define INITIAL_GRID_WIDTH 15
#define CHUNK_UPDATE_RATE 50
void drawGrid () {		

	Chunk renderedChunks[50];
	renderedChunks[0].coord[0] = 0;
	renderedChunks[0].coord[1] = 0;
	renderedChunks[0].cellsToTestCount = 0;

	int lineDistance, i, j, k, drawnPosX, drawnPosY, worldPosX, worldPosY, mouseRClickX, mouseRClickY, rectPosX, rectPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	int insert, insertIndexOfArrayX, insertIndexOfArrayY, insertIndexOfCellX, insertIndexOfCellY, frameCounter, mouseOffsetPerFrameX, mouseOffsetPerFrameY;
	double zoom;
	lineDistance = INITIAL_GRID_WIDTH;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	SetTargetFPS(60);
	insert = 1;
	mouseOffsetX = mouseOffsetY = 0;
	prevMouseOffsetX = prevMouseOffsetY = 0;
	worldPosX = worldPosY = 0;
	zoom = 1.000;
	frameCounter = CHUNK_UPDATE_RATE;
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
			mouseOffsetPerFrameX = mouseOffsetX - prevMouseOffsetX;
			mouseOffsetPerFrameY = mouseOffsetY - prevMouseOffsetY;
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			mouseOffsetPerFrameX = mouseOffsetPerFrameY = 0;
		}
		
		zoom = GetMouseWheelMove();
		lineDistance -= (int)zoom;


		BeginDrawing();
			
			ClearBackground(RAYWHITE);
			worldPosX += mouseOffsetPerFrameX;
			worldPosY += mouseOffsetPerFrameY; 


			drawnPosX = worldPosX % lineDistance; 
			drawnPosY = worldPosY % lineDistance;
			
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
			if (IsKeyPressed(32) && insert){
				insert = 0;
				initialTestedCells(&renderedChunks[0]);
			}
			if (insert) {
				if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
					mouseRClickX = GetMouseX();
					mouseRClickY = GetMouseY();
						
					//Used for finding index of clicked array
					insertIndexOfArrayX = ((mouseRClickX - worldPosX) / lineDistance) / ARR_SIZE;
					insertIndexOfArrayY = ((mouseRClickY - worldPosY) / lineDistance) / ARR_SIZE;

					if (mouseRClickX - worldPosX < 0)
						--insertIndexOfArrayX;
					if (mouseRClickY - worldPosY < 0)
						--insertIndexOfArrayY;
						
					//printf("X: %d, Y: %d\n", insertIndexOfArrayX, insertIndexOfArrayY);

					insertIndexOfCellX = (mouseRClickX - worldPosX) / lineDistance;
					insertIndexOfCellY = (mouseRClickY - worldPosY) / lineDistance;

					renderedChunks[0].cells[insertIndexOfCellY][insertIndexOfCellX].alive = !renderedChunks[0].cells[insertIndexOfCellY][insertIndexOfCellX].alive;
					renderedChunks[0].cellsToTest[renderedChunks[0].cellsToTestCount][0] = insertIndexOfCellX;
					renderedChunks[0].cellsToTest[renderedChunks[0].cellsToTestCount][1] = insertIndexOfCellY;
					renderedChunks[0].cellsToTestCount++;

				}
			} else {
				
				if (frameCounter == CHUNK_UPDATE_RATE) {
					testAliveNeighbors(&renderedChunks[0]);
					cellAliveState(&renderedChunks[0]);
					--frameCounter;
				} else {
					--frameCounter;
					if (frameCounter <= 0) {
						frameCounter = CHUNK_UPDATE_RATE;
					}
				}
			}
				for (i = 0; i < ARR_SIZE; ++i) {
					for (j = 0; j < ARR_SIZE; ++j) {
						if(renderedChunks[0].cells[i][j].alive) {
							//TODO: only render if coords are inside screen
							DrawRectangle(worldPosX + j * lineDistance, worldPosY + i * lineDistance, lineDistance, lineDistance, GRAY);

					 	}
					}
				}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
