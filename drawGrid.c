#include "drawGrid.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "arrays.h"
#include "helpers.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define INITIAL_GRID_WIDTH 15
#define CHUNK_UPDATE_RATE 5

void drawGrid (Chunk * chunkList, Chunk * lastChunkOfList) {		

	//Chunk *renderedChunks[50];

	int lineDistance, i, j, k, drawnPosX, drawnPosY, mouseRClickX, mouseRClickY, rectPosX, rectPosY;
	long long int worldPosX, worldPosY;
	int initMouseX, initMouseY, mouseOffsetX, mouseOffsetY, prevMouseOffsetX, prevMouseOffsetY;
	int insert, worldIndexOfArrayX, worldIndexOfArrayY, insertIndexOfCellX, insertIndexOfCellY, frameCounter, mouseOffsetPerFrameX, mouseOffsetPerFrameY;
	int renderedChunkCount, indexOfCurArr, rectX, rectY;
	int selectionStartX, selectionStartY, newCopy, selectionSizeX, selectionSizeY;
	int gridDrawnBool;
	int coordToFind[2];
	double zoom;
	Chunk * firstChunk, * lastChunk, * curChunk;
	int * selectedArea;

	selectedArea = NULL;
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
	renderedChunkCount = 0;
	newCopy = 0;
	gridDrawnBool = 1;

	if (chunkList != NULL) {
		firstChunk = chunkList;
		lastChunk = lastChunkOfList;
		curChunk = firstChunk;
		renderedChunkCount = 1;

	}

	while (!WindowShouldClose()) {
		

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_C)) {
			initMouseX = GetMouseX() - worldPosX;
			initMouseY = GetMouseY() - worldPosY;
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_C)) {
			prevMouseOffsetX = mouseOffsetX;
			prevMouseOffsetY = mouseOffsetY;
			mouseOffsetX = GetMouseX() - initMouseX;
			mouseOffsetY = GetMouseY() - initMouseY;
			mouseOffsetPerFrameX = mouseOffsetX - prevMouseOffsetX;
			mouseOffsetPerFrameY = mouseOffsetY - prevMouseOffsetY;
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_C)) {
			mouseOffsetPerFrameX = mouseOffsetPerFrameY = 0;
		}
		if (IsKeyDown(KEY_C) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			selectionStartX = GetMouseX();
			selectionStartY = GetMouseY();
			newCopy = 1;
		}

		
		zoom = GetMouseWheelMove();
		lineDistance -= (int)zoom;
		if (lineDistance < 1) {
			lineDistance = 1;
		}
		if (lineDistance <= 2) {
			gridDrawnBool = 0;

		}


		BeginDrawing();
			
			ClearBackground(RAYWHITE);
			worldPosX += mouseOffsetPerFrameX;
			worldPosY += mouseOffsetPerFrameY; 


			drawnPosX = worldPosX % lineDistance; 
			drawnPosY = worldPosY % lineDistance;
			
			if (gridDrawnBool) {
				while (drawnPosX < SCREEN_WIDTH) {
					if (drawnPosX <= SCREEN_WIDTH) {
						DrawLine(drawnPosX, 0, drawnPosX, SCREEN_HEIGHT, GRAY);
					}
					if (drawnPosY <= SCREEN_HEIGHT) {
						DrawLine(0, drawnPosY, SCREEN_WIDTH, drawnPosY, GRAY);
					}
	
					drawnPosX += lineDistance;
					drawnPosY += lineDistance;
				
				}
			}
			if (IsKeyPressed(32) && insert){
				insert = 0;
				curChunk = firstChunk;
				 while (1) {
					initialTestedCells(curChunk);
					if (curChunk->nextChunk != NULL)
						curChunk = curChunk->nextChunk;
					else
						break;
				}
			}
			if (insert) {
				if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
					mouseRClickX = GetMouseX();
					mouseRClickY = GetMouseY();
						
					worldIndexOfArrayX = indexOfChunkFromCoord(mouseRClickX, worldPosX, lineDistance);
					worldIndexOfArrayY = indexOfChunkFromCoord(mouseRClickY, worldPosY, lineDistance);
						

					coordToFind[0] = worldIndexOfArrayX;
					coordToFind[1] = worldIndexOfArrayY;
					//Fix to return pointer to chunk
					if (renderedChunkCount == 0 && chunkList == NULL) {
						firstChunk = malloc(sizeof(Chunk));

						firstChunk->coord[0] = worldIndexOfArrayX;
						firstChunk->coord[1] = worldIndexOfArrayY;

						firstChunk->cellsToTestCount = 0;
						initializeZeroArray(firstChunk->cells);
						firstChunk->nextChunk = NULL;

						lastChunk = firstChunk;
						curChunk = firstChunk;
						++renderedChunkCount;
					} else {
						if ((curChunk = findIndex(coordToFind, firstChunk)) == NULL) {
							lastChunk = renderChunk(&lastChunk, worldIndexOfArrayX, worldIndexOfArrayY);
							curChunk = lastChunk;
							++renderedChunkCount;
						}
					}

					insertIndexOfCellX = arrayIndexFromCoord(mouseRClickX, worldPosX, lineDistance);
					insertIndexOfCellY = arrayIndexFromCoord(mouseRClickY, worldPosY, lineDistance);

					curChunk->cells[insertIndexOfCellY][insertIndexOfCellX].alive = !curChunk->cells[insertIndexOfCellY][insertIndexOfCellX].alive;	
					curChunk->cellsToTest[curChunk->cellsToTestCount][0] = insertIndexOfCellX;
					curChunk->cellsToTest[curChunk->cellsToTestCount][1] = insertIndexOfCellY;
					curChunk->cellsToTestCount = curChunk->cellsToTestCount + 1;

				}
			} else {
				if (frameCounter == CHUNK_UPDATE_RATE) {
					//printf("%d\n", renderedChunkCount);
					curChunk = firstChunk;
					while (1) {
						testAliveNeighbors(curChunk, &lastChunk, firstChunk, &renderedChunkCount);

						if (curChunk->nextChunk != NULL) 
							curChunk = curChunk->nextChunk;
						else
							break;
					} 
					curChunk = firstChunk;
					while (1) {
						cellAliveState(curChunk);
						if (curChunk->nextChunk != NULL)
							curChunk = curChunk->nextChunk;
						else
							break;
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

			if (renderedChunkCount > 0) {
				curChunk = firstChunk;
				 do {
					for (j = 0; j < curChunk->cellsToTestCount; ++j) {
							//TODO: only render if coords are inside screen
							curChunk->cells[curChunk->cellsToTest[j][1]][curChunk->cellsToTest[j][0]].aliveNeighbors = 0;
							if (curChunk->cells[curChunk->cellsToTest[j][1]][curChunk->cellsToTest[j][0]].alive) {
								rectX = (lineDistance * ARR_SIZE * curChunk->coord[0]) + worldPosX + curChunk->cellsToTest[j][0] * lineDistance;
								rectY = (lineDistance * ARR_SIZE * curChunk->coord[1]) + worldPosY + curChunk->cellsToTest[j][1] * lineDistance;
								DrawRectangle(rectX, rectY, lineDistance, lineDistance, GRAY);
				 			}
					}
					if (curChunk->nextChunk != NULL)
						curChunk = curChunk->nextChunk;
					else 
						break;
				} while (1);
			}
			if (IsKeyPressed(KEY_G)) {
				gridDrawnBool = !gridDrawnBool;	
			}
			if (IsKeyDown(KEY_C) && newCopy) {
				drawSelectionRect(
					(((selectionStartX - worldPosX % lineDistance)/ lineDistance) * lineDistance) + worldPosX % lineDistance,
					(((selectionStartY - worldPosY % lineDistance)/ lineDistance) * lineDistance) + worldPosY % lineDistance,
					(((GetMouseX() - worldPosX % lineDistance)/ lineDistance + 1) * lineDistance) + worldPosX % lineDistance,
					(((GetMouseY() - worldPosY % lineDistance)/ lineDistance + 1) * lineDistance) + worldPosY % lineDistance
				);

			}
			if (IsKeyDown(KEY_C) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				newCopy = 0;

				if (selectedArea != NULL) {
					free(selectedArea);
				}

				selectedArea = copySelectionArea(selectionStartX, selectionStartY, 
							   GetMouseX(), GetMouseY(), firstChunk, &selectionSizeX, 
							   &selectionSizeY, worldPosX, worldPosY, lineDistance);

				
			}
			if (IsKeyDown(KEY_V) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				pasteSelectionArea(selectedArea, GetMouseX(), GetMouseY(), firstChunk, &lastChunk, selectionSizeX, selectionSizeY, worldPosX, worldPosY, lineDistance, &renderedChunkCount);
				

				
			}

		EndDrawing();
	}
}

