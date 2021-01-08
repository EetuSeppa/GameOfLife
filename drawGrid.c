/*
	
	TODO:
	-Change array of rendered chunks to linked list
		-Requires changes in every function

	-Implement pausing evolution and inserting cells by pressing space

	-Implemented copying and pasting
		-When c key is pressed and mouse is dragged selection is active
		-When c key is released cells inside selection get put in a temporary array
		-When v and left mouse is pressed this temporary array is pasted

	-Free empty arrays

	-If alive cell "collides" with corner or edge of array, start checking in next array.
		-If collision in edges happens, check if array is alive yet, if not malloc space for array and insert edge cells to cellsToCheck of new array. If array is already rendered, just do the latter.  	




	-v Needs logic for finding correct index of colliding array -v Implement placing of cells into other arrays besides center array.
		-Needs functionality for allocating memory for new arrays and giving arrays correct coordinates
		-Needed also: testing alive conditions of multiple arrays

	-v Implement smart checking of cells inside each array.
		-Needs faster algorithm for searching. (Is searching really needed?)

*/
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "arrays.c"
#include "helpers.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define INITIAL_GRID_WIDTH 15
#define CHUNK_UPDATE_RATE 40
void drawGrid () {		

	Chunk *renderedChunks[50];

	int lineDistance, i, j, k, drawnPosX, drawnPosY, worldPosX, worldPosY, mouseRClickX, mouseRClickY, rectPosX, rectPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	int insert, worldIndexOfArrayX, worldIndexOfArrayY, insertIndexOfCellX, insertIndexOfCellY, frameCounter, mouseOffsetPerFrameX, mouseOffsetPerFrameY;
	int renderedChunkIndex, indexOfCurArr, rectX, rectY;
	int coordToFind[2];
	double zoom;


	lineDistance = INITIAL_GRID_WIDTH;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Of Life");

	SetTargetFPS(60);
	insert = 1;
	mouseOffsetX = mouseOffsetY = 0;
	prevMouseOffsetX = prevMouseOffsetY = 0;
	mouseOffsetPerFrameX = mouseOffsetPerFrameY = 0;
	worldPosX = worldPosY = 0;
	zoom = 1.000;
	frameCounter = CHUNK_UPDATE_RATE;
	renderedChunkIndex = 0;

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
				for (i = 0; i < renderedChunkIndex; ++i) {
					initialTestedCells(renderedChunks[i]);
				}
			}
			if (insert) {
				if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
					mouseRClickX = GetMouseX();
					mouseRClickY = GetMouseY();
						
					//Used for finding index of clicked array
					worldIndexOfArrayX = ((mouseRClickX - worldPosX) / lineDistance) / ARR_SIZE;
					worldIndexOfArrayY = ((mouseRClickY - worldPosY) / lineDistance) / ARR_SIZE;

					if (mouseRClickX - worldPosX < 0)
						--worldIndexOfArrayX;
					if (mouseRClickY - worldPosY < 0)
						--worldIndexOfArrayY;
						

					coordToFind[0] = worldIndexOfArrayX;
					coordToFind[1] = worldIndexOfArrayY;
					if ((indexOfCurArr = findIndex(coordToFind,  renderedChunks, renderedChunkIndex)) == -1) {

						renderChunk(&renderedChunkIndex, renderedChunks, worldIndexOfArrayX, worldIndexOfArrayY);
						indexOfCurArr = renderedChunkIndex - 1;

					}
					insertIndexOfCellX = ((mouseRClickX - worldPosX) / lineDistance) % ARR_SIZE;
					insertIndexOfCellY = ((mouseRClickY - worldPosY) / lineDistance) % ARR_SIZE;
					if (mouseRClickX - worldPosX < 0)
						insertIndexOfCellX += 49;	
					if (mouseRClickY - worldPosY < 0)
						insertIndexOfCellY += 49;	

					renderedChunks[indexOfCurArr]->cells[insertIndexOfCellY][insertIndexOfCellX].alive = !renderedChunks[indexOfCurArr]->cells[insertIndexOfCellY][insertIndexOfCellX].alive;	
					renderedChunks[indexOfCurArr]->cellsToTest[renderedChunks[indexOfCurArr]->cellsToTestCount][0] = insertIndexOfCellX;
					renderedChunks[indexOfCurArr]->cellsToTest[renderedChunks[indexOfCurArr]->cellsToTestCount][1] = insertIndexOfCellY;
					renderedChunks[indexOfCurArr]->cellsToTestCount = renderedChunks[indexOfCurArr]->cellsToTestCount + 1;

				}
			} else {
				if (frameCounter == CHUNK_UPDATE_RATE) {
					for (i = 0; i < renderedChunkIndex; ++i) {
						testAliveNeighbors(renderedChunks[i], renderedChunks, &renderedChunkIndex);
					}
					for (i = 0; i < renderedChunkIndex; ++i) {
						cellAliveState(renderedChunks[i]);
					}
					--frameCounter;
					//getchar();	
				} else {
					--frameCounter;
					if (frameCounter <= 0) {
						frameCounter = CHUNK_UPDATE_RATE;
					}
				}
			}
			//DOESNT RUN FOR FOR EVERY CELL
				for (i = 0; i < renderedChunkIndex; ++i) {
					for (j = 0; j < renderedChunks[i]->cellsToTestCount; ++j) {
							//TODO: only render if coords are inside screen
							renderedChunks[i]->cells[renderedChunks[i]->cellsToTest[j][1]][renderedChunks[i]->cellsToTest[j][0]].aliveNeighbors = 0;
							if (renderedChunks[i]->cells[renderedChunks[i]->cellsToTest[j][1]][renderedChunks[i]->cellsToTest[j][0]].alive) {
								rectX = (lineDistance * ARR_SIZE * renderedChunks[i]->coord[0]) + worldPosX + renderedChunks[i]->cellsToTest[j][0] * lineDistance;
								rectY = (lineDistance * ARR_SIZE * renderedChunks[i]->coord[1]) + worldPosY + renderedChunks[i]->cellsToTest[j][1] * lineDistance;
								DrawRectangle(rectX, rectY, lineDistance, lineDistance, GRAY);
					 		}
					}
				}
		EndDrawing();
	}
}

int main (void) {
	drawGrid();

}
