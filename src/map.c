#include "map.h"

#include <stdio.h>

#include <ace/managers/blit.h>

#include "cross.h"
#include "cube.h"
#include "game.h"

UBYTE g_pMapData[MAP_WIDTH][MAP_HEIGHT] = {0};
UBYTE g_ubStartPointX = MAP_WIDTH;
UBYTE g_ubStartPointY = MAP_HEIGHT;
UBYTE g_ubStartPointCrossSide = 0;
UBYTE g_ubDestinationPointX = MAP_WIDTH;
UBYTE g_ubDestinationPointY = MAP_HEIGHT;
UBYTE g_ubDestinationPointCrossSide = 0;
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
	if ((g_ubStartPointX != MAP_WIDTH) && (g_ubStartPointY != MAP_HEIGHT)) {
		UWORD uwX = getMapCrossX(g_ubStartPointX);
		UWORD uwY = getMapCrossY(g_ubStartPointX, g_ubStartPointY);

		drawCube(
			uwX + g_pCubeCrossSideAdjust[g_ubStartPointCrossSide][0][0],
			uwY + g_pCubeCrossSideAdjust[g_ubStartPointCrossSide][0][1]
		);

		if (g_ubStartPointCrossSide & 1) {
			drawCross(uwX, uwY, g_pMapData[g_ubStartPointX][g_ubStartPointY]);
		}
	}
}

void undrawMapStartPoint() {
	if ((g_ubStartPointX != MAP_WIDTH) && (g_ubStartPointY != MAP_HEIGHT)) {
		UWORD uwX = getMapCrossX(g_ubStartPointX);
		UWORD uwY = getMapCrossY(g_ubStartPointX, g_ubStartPointY);

		undrawCross(uwX, uwY);
		drawCross(uwX, uwY, g_pMapData[g_ubStartPointX][g_ubStartPointY]);
	}
}

void drawMapDestinationPoint() {
	if ((g_ubDestinationPointX != MAP_WIDTH) && (g_ubDestinationPointY != MAP_HEIGHT) && g_pMapData[g_ubDestinationPointX][g_ubDestinationPointY] && !(g_ubDestinationPointCrossSide & 1)) {
		drawDestinationPoint(
			getMapCrossX(g_ubDestinationPointX),
			getMapCrossY(g_ubDestinationPointX, g_ubDestinationPointY),
			g_ubDestinationPointCrossSide
		);
	}
}

void undrawMapDestinationPoint() {
	if ((g_ubDestinationPointX != MAP_WIDTH) && (g_ubDestinationPointY != MAP_HEIGHT) && !(g_ubDestinationPointCrossSide & 1)) {
		undrawDestinationPoint(
			getMapCrossX(g_ubDestinationPointX),
			getMapCrossY(g_ubDestinationPointX, g_ubDestinationPointY)
		);

		if (g_pMapData[g_ubDestinationPointX][g_ubDestinationPointY]) {
			drawMapCross(g_ubDestinationPointX, g_ubDestinationPointY);
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
	fputc(g_ubStartPointX, pFile);
	fputc(g_ubStartPointY, pFile);
	fputc(g_ubStartPointCrossSide, pFile);
	fputc(g_ubDestinationPointX, pFile);
	fputc(g_ubDestinationPointY, pFile);
	fputc(g_ubDestinationPointCrossSide, pFile);

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

	g_ubStartPointX = fgetc(pFile);
	g_ubStartPointY = fgetc(pFile);
	g_ubStartPointCrossSide = fgetc(pFile);
	g_ubDestinationPointX = fgetc(pFile);
	g_ubDestinationPointY = fgetc(pFile);
	g_ubDestinationPointCrossSide = fgetc(pFile);

	fread(g_pMapData, CROSS_BYTE_SIZE, MAP_WIDTH * MAP_HEIGHT, pFile);

	fclose(pFile);

	logWrite("done\n");

	return 1;
}