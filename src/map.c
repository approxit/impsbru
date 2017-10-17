#include "map.h"

#include <stdio.h>

#include <ace/managers/blit.h>

#include "cross.h"
#include "cube.h"
#include "game.h"

UBYTE g_pMapData[MAP_WIDTH][MAP_HEIGHT] = {0};
UBYTE g_ubMapStartPointX = MAP_WIDTH;
UBYTE g_ubMapStartPointY = MAP_HEIGHT;
UBYTE g_ubMapStartPointCrossSide = 0;
UBYTE g_ubMapDestinationPointX = MAP_WIDTH;
UBYTE g_ubMapDestinationPointY = MAP_HEIGHT;
UBYTE g_ubMapDestinationPointCrossSide = 0;
BYTE g_pMapCursorShifts[2][CROSS_SIDE_COUNT][2] = {
	{
   		{0, -1}, // N
   		{1, -1}, // NE
		{1, 0}, // SE
		{0, 1}, // S
		{-1, 0}, // SW
   		{-1, -1} // NW
	},
	{
		{0, -1}, // N
		{1, 0}, // NE
		{1, 1}, // SE
		{0, 1}, // S
		{-1, 1}, // SW
		{-1, 0} // NW
	}
};

UWORD getMapCrossX(UBYTE ubCrossXIndex) {
	return CROSS_X_SHIFT * ubCrossXIndex;
}

UWORD getMapCrossY(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex) {
	return MAP_Y_OFFSET + ((ubCrossXIndex % 2) * CROSS_ODD_Y_SHIFT) + (CROSS_Y_SHIFT * ubCrossYIndex);
}

BYTE getMapShiftX(UBYTE ubMapCursorX, UBYTE ubCrossSide) {
	return g_pMapCursorShifts[getRowParity(ubMapCursorX)][ubCrossSide][0];
}

BYTE getMapShiftY(UBYTE ubMapCursorX, UBYTE ubCrossSide) {
	return g_pMapCursorShifts[getRowParity(ubMapCursorX)][ubCrossSide][1];
}

UBYTE getRowParity(UBYTE ubMapCursorX) {
	return ubMapCursorX & 1;
}

UBYTE getMapNeighborXRestrictive(UBYTE ubMapCursorX, UBYTE ubCrossSide) {
	BYTE bShiftX = getMapShiftX(ubMapCursorX, ubCrossSide);
	bShiftX += ubMapCursorX;
	if ((0 <= bShiftX) && (bShiftX < MAP_WIDTH)) {
		return bShiftX;
	}

	return MAP_WIDTH;
}

UBYTE getMapNeighborYRestrictive(UBYTE ubMapCursorX, UBYTE ubMapCursorY, UBYTE ubCrossSide) {
	BYTE bShiftY = getMapShiftY(ubMapCursorX, ubCrossSide);
	bShiftY += ubMapCursorY;
	if ((0 <= bShiftY) && (bShiftY < MAP_HEIGHT)) {
		return bShiftY;
	}

	return MAP_HEIGHT;
}

UBYTE getMapNeighborX(UBYTE ubMapCursorX, UBYTE ubCrossSide) {
	BYTE bShiftX = getMapShiftX(ubMapCursorX, ubCrossSide);
	if ((0 < bShiftX) && ((MAP_WIDTH - 1 - ubMapCursorX) < bShiftX)) {
		ubMapCursorX = MAP_WIDTH - 1;
	}
	else if ((bShiftX < 0) && (ubMapCursorX < -bShiftX)) {
		ubMapCursorX = 0;
	}
	else {
		ubMapCursorX += bShiftX;
	}

	return ubMapCursorX;
}

UBYTE getMapNeighborY(UBYTE ubMapCursorX, UBYTE ubMapCursorY, UBYTE ubCrossSide) {
	BYTE bShiftY = getMapShiftY(ubMapCursorX, ubCrossSide);
	if ((0 < bShiftY) && (MAP_HEIGHT - 1 - ubMapCursorY) < bShiftY) {
		ubMapCursorY = MAP_HEIGHT - 1;
	}
	else if ((bShiftY < 0) && (ubMapCursorY < -bShiftY)) {
		ubMapCursorY = 0;
	}
	else {
		ubMapCursorY += bShiftY;
	}

	return ubMapCursorY;
}

void drawMap() {
	for (UBYTE ubCrossXIndex = 0; ubCrossXIndex < MAP_WIDTH; ++ubCrossXIndex) {
		for (UBYTE ubCrossYIndex = 0; ubCrossYIndex < MAP_HEIGHT; ++ubCrossYIndex) {
			drawMapCross(ubCrossXIndex, ubCrossYIndex);
		}
	}

	drawMapStartPoint();
	drawMapDestinationPoint();
}

void undrawMap() {
	blitRect(
		g_pBufferManager->pBuffer, 0, MAP_Y_OFFSET,
		MAP_WIDTH * CROSS_X_SHIFT, MAP_HEIGHT * CROSS_Y_SHIFT + CROSS_ODD_Y_SHIFT,
		0
	);
}

void drawMapCross(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex) {
//	logWrite("drawMapCross: %ux%u\n", ubCrossXIndex, ubCrossYIndex);
	drawCross(
		getMapCrossX(ubCrossXIndex),
		getMapCrossY(ubCrossXIndex, ubCrossYIndex),
		g_pMapData[ubCrossXIndex][ubCrossYIndex]
	);
}

void undrawMapCross(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex) {
	undrawCross(
		getMapCrossX(ubCrossXIndex),
		getMapCrossY(ubCrossXIndex, ubCrossYIndex)
	);
}

void drawMapStartPoint() {
	if ((g_ubMapStartPointX != MAP_WIDTH) && (g_ubMapStartPointY != MAP_HEIGHT)) {
		UWORD uwX = getMapCrossX(g_ubMapStartPointX);
		UWORD uwY = getMapCrossY(g_ubMapStartPointX, g_ubMapStartPointY);

		drawCube(
			uwX + g_pCubeCrossSideAdjust[g_ubMapStartPointCrossSide][g_ubMapStartPointCrossSide][0],
			uwY + g_pCubeCrossSideAdjust[g_ubMapStartPointCrossSide][g_ubMapStartPointCrossSide][1]
		);

		if (g_ubMapStartPointCrossSide & 1) {
			drawCross(uwX, uwY, g_pMapData[g_ubMapStartPointX][g_ubMapStartPointY]);
		}
	}
}

void undrawMapStartPoint() {
	if ((g_ubMapStartPointX != MAP_WIDTH) && (g_ubMapStartPointY != MAP_HEIGHT)) {
		UWORD uwX = getMapCrossX(g_ubMapStartPointX);
		UWORD uwY = getMapCrossY(g_ubMapStartPointX, g_ubMapStartPointY);

		undrawCross(uwX, uwY);
		drawCross(uwX, uwY, g_pMapData[g_ubMapStartPointX][g_ubMapStartPointY]);
	}
}

void drawMapDestinationPoint() {
	if ((g_ubMapDestinationPointX != MAP_WIDTH) && (g_ubMapDestinationPointY != MAP_HEIGHT) && g_pMapData[g_ubMapDestinationPointX][g_ubMapDestinationPointY] && !(g_ubMapDestinationPointCrossSide & 1)) {
		drawDestinationPoint(
			getMapCrossX(g_ubMapDestinationPointX),
			getMapCrossY(g_ubMapDestinationPointX, g_ubMapDestinationPointY),
			g_ubMapDestinationPointCrossSide
		);
	}
}

void undrawMapDestinationPoint() {
	if ((g_ubMapDestinationPointX != MAP_WIDTH) && (g_ubMapDestinationPointY != MAP_HEIGHT) && !(g_ubMapDestinationPointCrossSide & 1)) {
		undrawDestinationPoint(
			getMapCrossX(g_ubMapDestinationPointX),
			getMapCrossY(g_ubMapDestinationPointX, g_ubMapDestinationPointY)
		);

		if (g_pMapData[g_ubMapDestinationPointX][g_ubMapDestinationPointY]) {
			drawMapCross(g_ubMapDestinationPointX, g_ubMapDestinationPointY);
		}
	}
}

UBYTE saveMapToFile(char *szFilePath) {
	logWrite("Saving %s... ", szFilePath);
	FILE *pFile = fopen(szFilePath, "w");
	if (!pFile) {
		logWrite("ERROR: can\'t open file!\n");
		return 0;
	}

	fputc(MAP_WIDTH, pFile);
	fputc(MAP_HEIGHT, pFile);
	fputc(g_ubMapStartPointX, pFile);
	fputc(g_ubMapStartPointY, pFile);
	fputc(g_ubMapStartPointCrossSide, pFile);
	fputc(g_ubMapDestinationPointX, pFile);
	fputc(g_ubMapDestinationPointY, pFile);
	fputc(g_ubMapDestinationPointCrossSide, pFile);

	fwrite(g_pMapData, CROSS_BYTE_SIZE, MAP_WIDTH * MAP_HEIGHT, pFile);

	fclose(pFile);

	logWrite("done\n");

	return 1;
}

UBYTE loadMapFromFile(char *szFilePath) {
	logWrite("Loading %s... ", szFilePath);
	FILE *pFile = fopen(szFilePath, "r");
	if (!pFile) {
		logWrite("ERROR: can\'t open file!\n");
		return 0;
	}

	UBYTE ubMapWidth = fgetc(pFile);
	UBYTE ubMapHeight = fgetc(pFile);

	if ((ubMapWidth != MAP_WIDTH) || (ubMapHeight != MAP_HEIGHT)) {
		logWrite("ERROR: Map dimensions mismatch! Map: %ux%u Engine: %ux%u\n", ubMapWidth, ubMapHeight, MAP_WIDTH, MAP_HEIGHT);
		return 0;
	}

	g_ubMapStartPointX = fgetc(pFile);
	g_ubMapStartPointY = fgetc(pFile);
	g_ubMapStartPointCrossSide = fgetc(pFile);
	g_ubMapDestinationPointX = fgetc(pFile);
	g_ubMapDestinationPointY = fgetc(pFile);
	g_ubMapDestinationPointCrossSide = fgetc(pFile);

	fread(g_pMapData, CROSS_BYTE_SIZE, MAP_WIDTH * MAP_HEIGHT, pFile);

	fclose(pFile);

	logWrite("done\n");

	return 1;
}